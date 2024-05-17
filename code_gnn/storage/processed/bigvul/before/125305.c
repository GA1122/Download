bool DocumentTimeline::serviceAnimations()
{
    TRACE_EVENT0("webkit", "DocumentTimeline::serviceAnimations");

    m_timing->cancelWake();

    double timeToNextEffect = std::numeric_limits<double>::infinity();
    bool didTriggerStyleRecalc = false;
    for (int i = m_players.size() - 1; i >= 0; --i) {
        double playerNextEffect;
        bool playerDidTriggerStyleRecalc;
        if (!m_players[i]->update(&playerNextEffect, &playerDidTriggerStyleRecalc))
            m_players.remove(i);
        didTriggerStyleRecalc |= playerDidTriggerStyleRecalc;
        if (playerNextEffect < timeToNextEffect)
            timeToNextEffect = playerNextEffect;
    }

    if (!m_players.isEmpty()) {
        if (timeToNextEffect < s_minimumDelay)
            m_timing->serviceOnNextFrame();
        else if (timeToNextEffect != std::numeric_limits<double>::infinity())
            m_timing->wakeAfter(timeToNextEffect - s_minimumDelay);
    }

    return didTriggerStyleRecalc;
}
