#include "Channel.hpp"
#include "cassert"

void test_channel()
{

    Channel channel("TESTING");
    std::string user("IM A REGULAR USER");
    std::string operator_user("IM A operator USER");

    channel.add_operator(operator_user);
    channel.add_user(user);

    // the user count is expected to be 2
    assert(channel.user_count() == 2);

    std::string topic ("BEST CHANNEL EFFAH");

    // set the topic of the channel this can only be done by the operator user
    channel.set_topic(operator_user, topic);

    std::cout << "The topic of the channel is = " << channel.get_topic() << std::endl;

    try 
    {
        channel.set_mode(Channel::modes::TOPIC_LOCK);
        channel.set_topic(user, "THIS should not be possible");
    } 
    catch (std::exception &e) {}

    // topic should bot be changed
    assert(channel.get_topic() == topic);

    channel.remove_operator(operator_user);
    channel.remove_user(user);

    // ths user count is expected to be 0
    assert(channel.user_count() == 0);
}