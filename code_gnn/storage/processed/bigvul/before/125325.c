void Player::setCurrentTime(double newCurrentTime)
{
    if (!std::isfinite(newCurrentTime))
        return;
    updateTimingState(newCurrentTime);
     m_timeline.serviceAnimations();
 }
