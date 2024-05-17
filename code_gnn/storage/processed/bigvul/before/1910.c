static void reds_migrate_channels_seamless(void)
{
    RedClient *client;

     
    client = reds_get_client();
    red_client_migrate(client);
}
