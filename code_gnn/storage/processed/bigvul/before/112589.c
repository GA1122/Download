MediaCanStartListener* Document::takeAnyMediaCanStartListener()
{
    HashSet<MediaCanStartListener*>::iterator slot = m_mediaCanStartListeners.begin();
    if (slot == m_mediaCanStartListeners.end())
        return 0;
    MediaCanStartListener* listener = *slot;
    m_mediaCanStartListeners.remove(slot);
    return listener;
}
