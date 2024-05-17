void WebPagePrivate::enterFullscreenForNode(Node* node)
{
#if ENABLE(VIDEO)
    if (!node || !node->hasTagName(HTMLNames::videoTag))
        return;

    MediaPlayer* player = static_cast<HTMLMediaElement*>(node)->player();
    if (!player)
        return;

    MediaPlayerPrivate* mmrPlayer = static_cast<MediaPlayerPrivate*>(player->implementation());
    if (!mmrPlayer)
        return;

    Platform::Graphics::Window* window = mmrPlayer->getWindow();
    if (!window)
        return;

    const char* contextName = mmrPlayer->mmrContextName();
    if (!contextName)
        return;

    mmrPlayer->setFullscreenWebPageClient(m_client);
    m_fullscreenNode = node;
    m_client->fullscreenStart(contextName, window, mmrPlayer->getWindowScreenRect());
#endif
}
