void red_channel_disconnect(RedChannel *channel)
{
    RingItem *link;
    RingItem *next;

    RING_FOREACH_SAFE(link, next, &channel->clients) {
        red_channel_client_disconnect(
            SPICE_CONTAINEROF(link, RedChannelClient, channel_link));
    }
}
