void Player::pause()
{
    if (m_paused)
        return;
    m_paused = true;
    updateTimingState(currentTime());
    cancelAnimationOnCompositor();
}
