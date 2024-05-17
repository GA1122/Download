bool Document::hasPendingForcedStyleRecalc() const
{
    return m_styleRecalcTimer.isActive() && m_pendingStyleRecalcShouldForce;
}
