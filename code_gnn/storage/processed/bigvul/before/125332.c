void CSSPendingAnimations::add(Player* player)
{
    ASSERT(player->source()->isAnimation());
    const double defaultStartTime = player->timeline().currentTime();
    m_pending.append(std::make_pair(player, defaultStartTime));
}
