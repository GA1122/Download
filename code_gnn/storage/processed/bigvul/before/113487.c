void WebPagePrivate::exitFullscreenForNode(Node* node)
{
#if ENABLE(VIDEO)
    if (m_fullscreenNode.get()) {
        m_client->fullscreenStop();
        m_fullscreenNode = 0;
    }

    if (!node || !node->hasTagName(HTMLNames::videoTag))
        return;

    MediaPlayer* player = static_cast<HTMLMediaElement*>(node)->player();
    if (!player)
        return;

    MediaPlayerPrivate* mmrPlayer = static_cast<MediaPlayerPrivate*>(player->implementation());
    if (!mmrPlayer)
        return;

    mmrPlayer->setFullscreenWebPageClient(0);
#endif
}
