static void red_channel_client_seamless_migration_done(RedChannelClient *rcc)
{
    rcc->wait_migrate_data = FALSE;

    pthread_mutex_lock(&rcc->client->lock);
    rcc->client->num_migrated_channels--;

     
    if (!rcc->client->num_migrated_channels) {
        rcc->client->during_target_migrate = FALSE;
        rcc->client->seamless_migrate = FALSE;
         
        main_dispatcher_seamless_migrate_dst_complete(rcc->client);
        if (rcc->latency_monitor.timer) {
            red_channel_client_start_ping_timer(rcc, PING_TEST_IDLE_NET_TIMEOUT_MS);
        }
    }
    pthread_mutex_unlock(&rcc->client->lock);
}
