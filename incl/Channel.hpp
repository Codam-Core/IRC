#pragma once

#include "includes.hpp"
#include <vector>

class Channel
{
    typedef std::string User;

    public:
        /**
         * @brief The available modes that a channel can have
         */
        enum modes
        {
            INVITE_ONLY = 1 << 0, // Is this channel invite only?
            TOPIC_LOCK = 1 << 1, // Are only operators allowed to change the topic of a channel
            PASSWORD = 1 << 2, // Has a password been set?
            LIMIT = 1 << 3, // Has a user limit been set
        };

        Channel(void) = delete;
        Channel(const std::string& name);
        ~Channel(void);

        // Channels shouldn't be copied, ore moved. once instantiated, they should be managed by the server and only accessed through references.
        Channel(const Channel& other) = delete;
        Channel(Channel&& other) = delete;
        
        /**
         * @brief Return the amount of users in the channel
         * @note When this function returns 0, the channel should be deleted from the server's list of channels
         * 
         * @return int 
         */
        int user_count() const;

        /**
         * @brief Add/Remove a regular user to/from the channel
         * 
         * @param user 
         */
        void add_user(const User& user);
        void remove_user(const User& user);

        /**
         * @brief Add/Remove an operator to/from the channel
         * 
         * @param user 
         */
        void add_operator(const User& user);
        void remove_operator(const User& user);

        /**
         * @brief Get the name of the channel
         * 
         * @return std::string
         */
        std::string get_name() const;

        /**
         * @brief Get the topic of the channel
         * 
         * @return std::string 
         */
        std::string get_topic() const;

        /**
         * @brief Set the topic of the channel
         * @note Only an operator is allowed to set a topic when channel has mode TOPIC_LOCK enabled
         * @throws std::invalid_argument if the user is not an operator of the channel
         * 
         * @param user
         * @param topic 
         */
        void set_topic(const User& user, const std::string& topic);

        /**
         * @brief Check whether a mode has been set
         * 
         * @param mode
         * @return true when the mode has been set
         */
        bool has_mode(modes mode) const;

        /**
         * @brief Activate/Deactivate a mode
         * 
         * @param mode
         * @return true when the mode has been set
         */
        void set_mode(modes mode);
        void unset_mode(modes mode);

    private:
        
        // the name of the channel, withouth the '#' prefix
        // @todo maybe change so that it includes the '#'
        const std::string _name;

        // the topic of the channel, which is a small description of what the channel is about. It can be empty, but it should be set by an operator.
        std::string _topic;

        std::vector<User> _users;
        std::vector<User> _operators;

        // this integer stores all the current active modes on a channel
        unsigned int _mode;
};