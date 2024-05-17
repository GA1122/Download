void red_channel_set_common_cap(RedChannel *channel, uint32_t cap)
{
    add_capability(&channel->local_caps.common_caps, &channel->local_caps.num_common_caps, cap);
}
