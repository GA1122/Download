void red_channel_client_shutdown(RedChannelClient *rcc)
{
    if (rcc->stream && !rcc->stream->shutdown) {
        rcc->channel->core->watch_remove(rcc->stream->watch);
        rcc->stream->watch = NULL;
        shutdown(rcc->stream->socket, SHUT_RDWR);
        rcc->stream->shutdown = TRUE;
    }
}
