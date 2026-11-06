#pragma once

#include "includes.hpp"

class Client;

class Channel
{
    private:
		const std::string			_name; // the name of the channel, with the '#' prefix, lowercase
        std::string					_topic; // the topic of the channel, which is a small description of what the channel is about. It can be empty, but it should be set by an operator.
		std::map<Client *, bool>	_members; // key = the member, value = is this member an operator?
		std::set<Client *>			_invited; // who may join while +i is on
        unsigned int 				_mode = 0; // this integer bitflag stores all the current active modes on a channel
		std::string					_key;  // the password, when +k is on
		size_t						_limit = 0; // max members, when +l is on

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

        // Channels shouldn't be copied, or moved. once instantiated, they should be managed by the server and only accessed through references.
        Channel(const Channel& other) = delete;
        Channel(Channel&& other) = delete;
        
		// members
		void add_member(Client *client, bool is_operator);
		void remove_member(Client *client);
		bool is_member(Client *client) const;
		bool is_operator(Client *client) const;
		void set_operator(Client *client, bool is_operator);   // for MODE +o / -o
		size_t member_count() const;
		const std::map<Client *, bool> &get_members() const;   // for "send to everyone"

		// invites
		void add_invite(Client *client);
		void remove_invite(Client *client);
		bool is_invited(Client *client) const;

		// key and limit
		const std::string &get_key() const;
		void set_key(const std::string &key);
		size_t get_limit() const;
		void set_limit(size_t limit);

		// modes
		bool has_mode(modes mode) const;
		void set_mode(modes mode);
		void unset_mode(modes mode);

		// getters and setters
		std::string get_name() const;
		std::string get_topic() const;
		void set_topic(const std::string &topic);
};


// TODO later:
// add this to each function:
// /**
//  * @brief Add/Remove an operator to/from the channel
//  * 
//  * @param client 
//  */

// or more complex like this if nessesary:
// /**
// * @brief Set the topic of the channel
// * @note Only an operator is allowed to set a topic when channel has mode TOPIC_LOCK enabled
// * @param client
// * @param topic 
// */