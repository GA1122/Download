void Player::reverse()
{
    if (!m_playbackRate)
        return;
    if (m_content) {
        if (m_playbackRate > 0 && currentTime() > sourceEnd())
            setCurrentTime(sourceEnd());
        else if (m_playbackRate < 0 && currentTime() < 0)
            setCurrentTime(0);
    }
    setPlaybackRate(-m_playbackRate);
    unpause();
}
