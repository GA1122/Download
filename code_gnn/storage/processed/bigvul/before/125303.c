size_t DocumentTimeline::numberOfActiveAnimationsForTesting() const
{
    if (isNull(m_zeroTime))
        return 0;
    if (isNull(m_zeroTime))
        return 0;
    size_t count = 0;
    for (size_t i = 0; i < m_players.size(); ++i) {
        const TimedItem* timedItem = m_players[i]->source();
        if (m_players[i]->hasStartTime())
            count += (timedItem && (timedItem->isCurrent() || timedItem->isInEffect()));
    }
    return count;
}
