static ChromeClient& emptyChromeClient()
{
    DEFINE_STATIC_LOCAL(EmptyChromeClient, client, (EmptyChromeClient::create()));
    return client;
}
