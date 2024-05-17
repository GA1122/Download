double Player::currentTime()
{
    updateCurrentTimingState();
    if (m_held)
        return m_holdTime;
    return currentTimeWithLag();
}
