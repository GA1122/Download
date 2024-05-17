 void Player::setSource(TimedItem* newSource)
{
    if (m_content == newSource)
        return;
    double storedCurrentTime = currentTime();
    if (m_content)
        m_content->detach();
    if (newSource) {
        if (newSource->player())
            newSource->detach();
        newSource->attach(this);
    }
    m_content = newSource;
    updateTimingState(storedCurrentTime);
}
