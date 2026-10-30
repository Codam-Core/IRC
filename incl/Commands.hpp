#pragma once
#include "Server.hpp"

class Commands
{

    typedef std::string Command;
    typedef void(Commands::*CommandFn)(const std::string&);

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
        void handle_command(const std::string& cmd, const std::string& args);

    private:

        void INVITE(const std::string &args);
        void NICK(const std::string &args);
        void PRIVMSG(const std::string &args);
        void TOPIC(const std::string &args);
        void JOIN(const std::string &args);

        static const std::map<Command, CommandFn> _commands;
        Server& _server;
};


