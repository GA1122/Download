void red_client_set_migration_seamless(RedClient *client)  
{
    RingItem *link;
    pthread_mutex_lock(&client->lock);
    client->seamless_migrate = TRUE;
     
    RING_FOREACH(link, &client->channels) {
        RedChannelClient *rcc = SPICE_CONTAINEROF(link, RedChannelClient, client_link);
        red_channel_client_set_migration_seamless(rcc);
    }
    pthread_mutex_unlock(&client->lock);
}
