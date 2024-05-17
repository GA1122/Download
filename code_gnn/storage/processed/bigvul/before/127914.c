void DocumentThreadableLoader::redirectBlocked()
{
    ThreadableLoaderClient* client = m_client;
    clear();
    client->didFailRedirectCheck();
}
