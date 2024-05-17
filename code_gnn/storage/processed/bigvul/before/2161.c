int red_channel_is_connected(RedChannel *channel)
{
    return channel && (channel->clients_num > 0);
}
