void TextTrackLoader::fileFailedToParse()
{
    m_state = Failed;

    if (!m_cueLoadTimer.isActive())
        m_cueLoadTimer.startOneShot(0, BLINK_FROM_HERE);

    cancelLoad();
}
