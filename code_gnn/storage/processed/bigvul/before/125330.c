void Player::updateCurrentTimingState()
{
    if (m_held) {
        updateTimingState(m_holdTime);
    } else {
        updateTimingState(currentTimeWithLag());
        if (m_held && limited(m_holdTime))
            m_holdTime = m_playbackRate < 0 ? 0 : sourceEnd();
    }
}
