bool Player::update(double* timeToEffectChange, bool* didTriggerStyleRecalc)
{
    if (!m_content) {
        if (timeToEffectChange)
            *timeToEffectChange = std::numeric_limits<double>::infinity();
        if (didTriggerStyleRecalc)
            *didTriggerStyleRecalc = false;
        return false;
    }

    double inheritedTime = isNull(m_timeline.currentTime()) ? nullValue() : currentTime();
    bool didTriggerStyleRecalcLocal = m_content->updateInheritedTime(inheritedTime);

    if (timeToEffectChange)
        *timeToEffectChange = m_content->timeToEffectChange();
    if (didTriggerStyleRecalc)
        *didTriggerStyleRecalc = didTriggerStyleRecalcLocal;
    return m_content->isCurrent() || m_content->isInEffect();
}
