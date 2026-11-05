#pragma once
#include "includes.hpp"
#include "colours.hpp"

// to prevent circular dependency, we forward declare the Server class here, and include it in Commands.cpp
class Server;
class Client;

class Commands
{

    typedef std::string Command;
    typedef void(Commands::*CommandFn)(Client &client, const std::vector<std::string>& args);

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
        void INVITE(Client &client, const std::vector<std::string>& args);
        void NICK(Client &client, const std::vector<std::string>& args);
        void PRIVMSG(Client &client, const std::vector<std::string>& args);
        void TOPIC(Client &client, const std::vector<std::string>& args);
        void JOIN(Client &client, const std::vector<std::string>& args);

        static const std::map<Command, CommandFn> _commands;
        Server& _server;
};


