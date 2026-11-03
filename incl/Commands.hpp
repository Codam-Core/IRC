#pragma once
#include "includes.hpp"
#include "colours.hpp"

// to prevent circular dependency, we forward declare the Server class here, and include it in Commands.cpp
class Server;

class Commands
{

    typedef std::string Command;
    typedef void(Commands::*CommandFn)(const std::vector<std::string>& args);

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
        void handle_command(const std::string& cmd, const std::vector<std::string>& args);

    private:

        void INVITE(const std::vector<std::string>& args);
        void NICK(const std::vector<std::string>& args);
        void PRIVMSG(const std::vector<std::string>& args);
        void TOPIC(const std::vector<std::string>& args);
        void JOIN(const std::vector<std::string>& args);

        static const std::map<Command, CommandFn> _commands;
        Server& _server;
};


