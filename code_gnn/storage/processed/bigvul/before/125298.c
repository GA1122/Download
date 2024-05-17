Player* DocumentTimeline::createPlayer(TimedItem* child)
{
    RefPtr<Player> player = Player::create(*this, child);
    Player* result = player.get();
    m_players.append(player.release());
    if (m_document->view())
        m_timing->serviceOnNextFrame();
    return result;
}
