static void reds_mig_target_client_add(RedClient *client)
{
    RedsMigTargetClient *mig_client;

    spice_assert(reds);
    spice_info(NULL);
    mig_client = spice_malloc0(sizeof(RedsMigTargetClient));
    mig_client->client = client;
    ring_init(&mig_client->pending_links);
    ring_add(&reds->mig_target_clients, &mig_client->link);
    reds->num_mig_target_clients++;

}
