#include "Commands.hpp"
#include "Server.hpp"
#include <queue>
#include <unordered_map>

void static mode_string(Client& client, Channel* channel, CommandStream& stream)
{
    std::string mode_str = "+";
    std::vector<std::string> mode_params;

    if (channel->has_mode(Channel::INVITE_ONLY))
        mode_str += "i";
    if (channel->has_mode(Channel::TOPIC_LOCK))
        mode_str += "t";
    if (channel->has_mode(Channel::PASSWORD))
    {
        mode_str += "k";
        mode_params.push_back(channel->get_key());
    }
    if (channel->has_mode(Channel::LIMIT))
    {
        mode_str += "l";
        mode_params.push_back(std::to_string(channel->get_limit()));
    }

    // assemble the MODE command response
    stream.prefix().code(324).client(client).parameter(channel->get_name()).parameter(mode_str).parameter(mode_params).finish();
}

std::vector<std::pair<char, bool>> static parse_mode(const std::string& mode_str)
{
    std::vector<std::pair<char, bool>> mode_map;
    bool adding = true; // true for adding modes, false for removing

    for (auto& c : mode_str)
    {
        if (c == '+')
            adding = true;
        else if (c == '-')
            adding = false;
        else
            mode_map.push_back(std::make_pair(c, adding));
    }
    return mode_map;
}

bool static set_or_unset_mode(Channel* channel, Channel::modes mode, bool adding)
{
    // true if the mode is NOT yet in the desired state, so it really changes
    bool changed = channel->has_mode(mode) != adding;

    // apply the change either way since it doesn't affect the state if it's already set to the desired state
    if (adding)
        channel->set_mode(mode);
    else
        channel->unset_mode(mode);

    return changed;
}

bool static handle_key_mode(Client& client, Channel* channel, bool adding, std::queue<std::string>& mode_arguments, CommandStream& stream)
{
    // If adding a key, we need to ensure that a key argument is provided.
    if (adding && mode_arguments.empty())
    {
        stream.prefix().code(461).client(client).command().message("Not enough parameters for +k").finish();
        return false;
    }

    // Set or unset the key mode on the channel based on the adding flag
    bool changed = set_or_unset_mode(channel, Channel::PASSWORD, adding);

    // If adding a key, set the key to the provided argument; otherwise, clear the key.
    if (adding)
    {
        // true if the key is different from the current one
        changed = channel->get_key() != mode_arguments.front();
        channel->set_key(mode_arguments.front());
    }
    else
        channel->set_key("");
    return changed;
}

bool static handle_limit_mode(Client& client, Channel* channel, bool adding, std::queue<std::string>& mode_arguments, CommandStream& stream)
{
    if (adding)
    {
        // If adding a limit, we need to ensure that a limit argument is provided.
        if (mode_arguments.empty())
        {
            stream.prefix().code(461).client(client).command().message("Not enough parameters for +l").finish();
            return false;
        }
        // Try to convert the argument to an unsigned long and set the limit; if it fails, send an error message.
        try
        {
            size_t limit = std::stoul(mode_arguments.front());
            channel->set_limit(limit);
        }
        catch (const std::exception& e)
        {
            stream.prefix().code(461).client(client).command().message("Invalid parameter for +l").finish();
            return false;
        }
    }
    else
        channel->set_limit(0);

    // Set or unset the limit mode on the channel based on the adding flag
    return set_or_unset_mode(channel, Channel::LIMIT, adding);
}

bool static handle_operator_mode(Client& client, Channel* channel, bool adding, std::queue<std::string>& mode_arguments, CommandStream& stream, Server& server)
{
    if (mode_arguments.empty())
    {
        stream.prefix().code(461).client(client).command().message("Not enough parameters for +o/-o").finish();
        return false;
    }

    std::string target_nickname = mode_arguments.front();

    Client* target = server.fetch_client_by_nickname(target_nickname);
    if (!target)
    {
        stream.prefix().code(401).client(client).parameter(target_nickname).message("No such user").finish();
        return false;
    }
    if (!channel->is_member(target))
    {
        stream.prefix().code(441).client(client).parameter(target_nickname).parameter(channel->get_name()).message("User not in channel").finish();
        return false;
    }

    // true if the operator status is already set to the desired state
    bool changed = channel->is_operator(target) != adding;

    if (adding)
        channel->set_operator(target, true);
    else
        channel->set_operator(target, false);

    return changed;
}

void Commands::MODE(Client &client, const std::vector<std::string>& args, CommandStream& stream)
{
    std::queue<std::string> mode_arguments;
    print_info(client, "MODE", args);

    if (args.size() < 1)
    {
        not_enough_parameters(client, stream);
        return;
    }

    // queue the mode arguments: everything after the mode string (args[0] = channel, args[1] = modes)
    for (size_t i = 2; i < args.size(); i++)
        mode_arguments.push(args[i]);

    // fetch the channel from the server
    Channel* channel = _server.find_channel(args[0]);

    // check whether the channel exists. If not, send back an error message.
    if (channel == nullptr)
    {
        stream.prefix().code(403).client(client).parameter(args[0]).message("Channel doesn't exist").finish();
        return;
    }
    if (!channel->is_member(&client))
    {
        stream.prefix().code(442).client(client).parameter(args[0]).message("You're not on that channel").finish();
        return;
    }

    // check whether the client requests the current mode settings for the channel or wants to change them
    if (args.size() == 1)
    {
        // If only the channel name is provided, return the current mode settings for that channel.
        mode_string(client, channel, stream);
        return;
    }

    // check if the client is an operator of the channel. Only operators can change channel modes.
    if (!channel->is_operator(&client))
    {
        stream.prefix().code(482).client(client).parameter(args[0]).message("You're not channel operator").finish();
        return;
    }

    // parse the mode string into a vector of mode changes
    std::vector<std::pair<char, bool>> mode_map = parse_mode(args[1]);

    // apply the mode changes to the channel
    for (auto& [mode, adding] : mode_map)
    {
        switch (mode)
        {
            case 'i':
                if (set_or_unset_mode(channel, Channel::modes::INVITE_ONLY, adding))
                    this->register_mode_change(std::string(1, mode), adding);
                break;
            case 't':
                if (set_or_unset_mode(channel, Channel::modes::TOPIC_LOCK, adding))
                    this->register_mode_change(std::string(1, mode), adding);
                break;
            case 'k':
                if (handle_key_mode(client, channel, adding, mode_arguments, stream))
                    this->register_mode_change(std::string(1, mode), adding, adding ? std::optional<std::string>(mode_arguments.front()) : std::nullopt);
                if (!mode_arguments.empty())
                    mode_arguments.pop();
                break;
            case 'l':
                if (handle_limit_mode(client, channel, adding, mode_arguments, stream))
                    this->register_mode_change(std::string(1, mode), adding, adding ? std::optional<std::string>(mode_arguments.front()) : std::nullopt);
                if (adding && !mode_arguments.empty())
                    mode_arguments.pop();
                break;
            case 'o':
                if (handle_operator_mode(client, channel, adding, mode_arguments, stream, _server))
                    this->register_mode_change(std::string(1, mode), adding, mode_arguments.front());
                if (!mode_arguments.empty())
                    mode_arguments.pop();
                break;
            default:
                stream.prefix().code(472).client(client).parameter(std::string(1, mode)).message("is unknown mode char to me").finish();
        }
    }

    // broadcast the mode changes to all members of the channel
    this->broadcast_mode_changes(channel, client);
}