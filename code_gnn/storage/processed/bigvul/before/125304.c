void DocumentTimeline::pauseAnimationsForTesting(double pauseTime)
{
    for (size_t i = 0; i < m_players.size(); i++)
        m_players[i]->pauseForTesting(pauseTime);
    serviceAnimations();
}
