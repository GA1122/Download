 void Player::setStartTime(double newStartTime)
// void Player::setStartTime(double newStartTime, bool serviceAnimations)
  {
      if (!std::isfinite(newStartTime))
          return;
      updateCurrentTimingState();  
      m_startTime = newStartTime;
      if (!m_held)
          updateCurrentTimingState();
    m_timeline.serviceAnimations();
//     if (serviceAnimations)
//         m_timeline.serviceAnimations();
//     else
//         update();
  }