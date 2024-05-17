double Player::currentTimeWithoutLag() const
{
    if (isNull(m_startTime))
        return 0;
    double timelineTime = m_timeline.currentTime();
    if (isNull(timelineTime))
        timelineTime = 0;
    return (timelineTime - m_startTime) * m_playbackRate;
}
