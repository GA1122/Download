void WebPagePrivate::applySizeOverride(int overrideWidth, int overrideHeight)
{
    m_client->requestUpdateViewport(overrideWidth, overrideHeight);
}
