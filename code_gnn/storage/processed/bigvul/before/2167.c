static void red_channel_pipes_create_batch(RedChannel *channel,
                                new_pipe_item_t creator, void *data,
                                rcc_item_t callback)
{
    RingItem *link;
    RedChannelClient *rcc;
    PipeItem *item;
    int num = 0;

    RING_FOREACH(link, &channel->clients) {
        rcc = SPICE_CONTAINEROF(link, RedChannelClient, channel_link);
        item = (*creator)(rcc, data, num++);
        if (callback) {
            (*callback)(rcc, item);
        }
    }
}
