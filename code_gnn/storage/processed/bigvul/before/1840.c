static inline void on_activating_ticketing(void)
{
    if (!ticketing_enabled && reds_main_channel_connected()) {
        spice_warning("disconnecting");
        reds_disconnect();
    }
}
