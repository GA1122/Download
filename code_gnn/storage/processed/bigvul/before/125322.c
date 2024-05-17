void Player::pauseForTesting(double pauseTime)
{
    RELEASE_ASSERT(!paused());
    updateTimingState(pauseTime);
    if (!m_isPausedForTesting && hasActiveAnimationsOnCompositor())
        toAnimation(m_content.get())->pauseAnimationForTestingOnCompositor(currentTime());
    m_isPausedForTesting = true;
    pause();
}
