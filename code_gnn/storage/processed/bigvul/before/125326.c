void Player::setPlaybackRate(double playbackRate)
{
    if (!std::isfinite(playbackRate))
        return;
    double storedCurrentTime = currentTime();
    m_playbackRate = playbackRate;
    updateTimingState(storedCurrentTime);
    m_timeline.serviceAnimations();
}
