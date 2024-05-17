void Player::play()
{
    unpause();
    if (!m_content)
        return;
    double currentTime = this->currentTime();
    if (m_playbackRate > 0 && (currentTime < 0 || currentTime >= sourceEnd()))
        setCurrentTime(0);
    else if (m_playbackRate < 0 && (currentTime <= 0 || currentTime > sourceEnd()))
        setCurrentTime(sourceEnd());
}
