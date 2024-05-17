static void migrate_timeout(void *opaque)
{
    spice_info(NULL);
    spice_assert(reds->mig_wait_connect || reds->mig_wait_disconnect);
    if (reds->mig_wait_connect) {
         
        main_channel_migrate_cancel_wait(reds->main_channel);
         
        reds_mig_target_client_disconnect_all();
        reds_mig_cleanup();
    } else {
        reds_mig_disconnect();
    }
}
