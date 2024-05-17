static void red_channel_peer_on_out_msg_done(void *opaque)
{
    RedChannelClient *rcc = (RedChannelClient *)opaque;

    rcc->send_data.size = 0;
    red_channel_client_release_sent_item(rcc);
    if (rcc->send_data.blocked) {
        rcc->send_data.blocked = FALSE;
        rcc->channel->core->watch_update_mask(rcc->stream->watch,
                                         SPICE_WATCH_EVENT_READ);
    }

    if (red_channel_client_urgent_marshaller_is_active(rcc)) {
        red_channel_client_restore_main_sender(rcc);
        spice_assert(rcc->send_data.header.data != NULL);
        red_channel_client_begin_send_message(rcc);
    } else {
        if (rcc->latency_monitor.timer && !rcc->send_data.blocked && rcc->pipe_size == 0) {
             
            red_channel_client_restart_ping_timer(rcc);
        }
    }

}
