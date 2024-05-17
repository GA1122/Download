void red_channel_push_set_ack(RedChannel *channel)
{
    red_channel_pipes_add_type(channel, PIPE_ITEM_TYPE_SET_ACK);
}
