PassRefPtr<Player> Player::create(DocumentTimeline& timeline, TimedItem* content)
{
    return adoptRef(new Player(timeline, content));
}
