void reds_on_client_seamless_migrate_complete(RedClient *client)
{
    spice_debug(NULL);
    if (!reds_find_client(client)) {
        spice_info("client no longer exists");
        return;
    }
    main_channel_migrate_dst_complete(red_client_get_main(client));
}
