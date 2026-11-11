#include "Channel.hpp"
#include "Client.hpp"
#include "cassert"

// a Client needs a real fd; an unconnected socket is enough for testing
static Client make_client()
{
    return Client(ConnectedSocket(Fd(socket(AF_INET, SOCK_STREAM, 0))));
}

void test_channel()
{
    Client op = make_client();
    Client user = make_client();
    Channel channel("#testing");

    // members and operators
    channel.add_member(&op, true);
    channel.add_member(&user, false);
    assert(channel.member_count() == 2);
    assert(channel.is_member(&op) && channel.is_member(&user));
    assert(channel.is_operator(&op));
    assert(!channel.is_operator(&user));

    channel.set_operator(&user, true);
    assert(channel.is_operator(&user));

    // topic
    channel.set_topic("BEST CHANNEL EFFAH");
    assert(channel.get_topic() == "BEST CHANNEL EFFAH");

    // modes
    channel.set_mode(Channel::TOPIC_LOCK);
    channel.set_mode(Channel::INVITE_ONLY);
    assert(channel.has_mode(Channel::TOPIC_LOCK) && channel.has_mode(Channel::INVITE_ONLY));
    channel.unset_mode(Channel::TOPIC_LOCK);
    assert(!channel.has_mode(Channel::TOPIC_LOCK) && channel.has_mode(Channel::INVITE_ONLY));

    // invites
    channel.add_invite(&user);
    assert(channel.is_invited(&user));
    channel.remove_invite(&user);
    assert(!channel.is_invited(&user));

    // key and limit
    channel.set_key("secret");
    assert(channel.get_key() == "secret");
    channel.set_limit(5);
    assert(channel.get_limit() == 5);

    // removing
    channel.remove_member(&op);
    channel.remove_member(&user);
    assert(channel.member_count() == 0);
}
