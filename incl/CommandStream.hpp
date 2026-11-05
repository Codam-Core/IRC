#pragma once
#include "Client.hpp"
#include "Channel.hpp"

class Server;

class CommandStream: public std::stringstream
{
    public:
        CommandStream(const std::string& command, Server& server);
        ~CommandStream();

        // assignment op
        CommandStream operator=(const CommandStream& other);

        // A command stream can be copied.
        CommandStream(const CommandStream& other);

        /**
         * @brief add a server prefix to the command stream
         * 
         * @return CommandStream& 
         */
        CommandStream& prefix();

        /**
         * @brief add a user prefix to the command stream
         * 
         * @return CommandStream&
         */
        CommandStream& prefix(const Client& client);

        /**
         * @brief add a channel prefix to the command stream
         * 
         * @return CommandStream&
         */
        CommandStream& prefix(Client& client, Channel& channel);

        /**
         * @brief add a return code to the command stream
         * 
         * @param int the return code
         */
        CommandStream& code(int code);

        /**
         * @brief add a client to the command stream
         * 
         * @param Client& the client
         */
        CommandStream& client(const Client& client);

        /**
         * @brief Add a parameter to the command stream
         * 
         * @param std::string the parameter
         * @return CommandStream&
         */
        CommandStream& parameter(const std::string& param);

        /**
         * @brief Add a parameter to the command stream
         * 
         * @param std::vector<std::string> the parameters
         * @return CommandStream&
         */
        CommandStream& parameter(const std::vector<std::string>& params);

        /**
         * @brief Add a message to the command stream
         * 
         * @param std::string the message
         * @return CommandStream&
         */
        CommandStream& message(const std::string& msg);

        /**
         * @brief add the command to the command stream
         * 
         * @return CommandStream&
         */
        CommandStream& command();

        /**
         * @brief finish the command stream by adding the CRLF
         */
        void finish();

        /**
         * @brief Send all the responses in the command stream to a client
         */
        void send(Client& client);

    private:
        std::vector<std::string> responses;
        std::string _command;
        Server& _server;
};