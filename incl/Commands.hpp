#pragma once
#include "includes.hpp"
#include "colours.hpp"
#include "CommandStream.hpp"

// to prevent circular dependency, we forward declare the Server class here, and include it in Commands.cpp
class Server;
class Client;
class CommandStream;

class Commands
{

    typedef std::string Command;
    typedef void(Commands::*CommandFn)(Client &client, const std::vector<std::string>& args, CommandStream& stream);

    public:
        Commands(void) = delete;
        Commands(const Commands& other) = delete;
        Commands(Commands&& other) = delete;

        Commands(Server& server);

        /**
         * @brief Command handler will process a specified command with the given arguments
         * 
         * @param cmd 
         * @param args 
         */
        void handle_command(Client &client, const std::string& cmd, const std::vector<std::string>& args);

    private:
        void INVITE(Client &client, const std::vector<std::string>& args, CommandStream& stream);
        void NICK(Client &client, const std::vector<std::string>& args, CommandStream& stream);
        void PRIVMSG(Client &client, const std::vector<std::string>& args, CommandStream& stream);
        void TOPIC(Client &client, const std::vector<std::string>& args, CommandStream& stream);
        void JOIN(Client &client, const std::vector<std::string>& args, CommandStream& stream);
        void CAP(Client &client, const std::vector<std::string>& args, CommandStream& stream);
        void PASS(Client &client, const std::vector<std::string>& args, CommandStream& stream);
        void USER(Client &client, const std::vector<std::string>& args, CommandStream& stream);

        // helper functions
        void not_enough_parameters(Client &client, CommandStream& stream);

        static const std::map<Command, CommandFn> _commands;
        Server& _server;
};


