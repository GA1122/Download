void reds_on_client_semi_seamless_migrate_complete(RedClient *client)
{
    MainChannelClient *mcc;

    spice_info("%p", client);
    mcc = red_client_get_main(client);

    main_channel_push_init(mcc, red_dispatcher_count(),
                           reds->mouse_mode, reds->is_client_mouse_allowed,
                           reds_get_mm_time() - MM_TIME_DELTA,
                           red_dispatcher_qxl_ram_size());
    reds_link_mig_target_channels(client);
    main_channel_migrate_dst_complete(mcc);
}
