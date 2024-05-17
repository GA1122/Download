double Player::currentTimeWithLag() const
{
    ASSERT(!m_held);
    double time = currentTimeWithoutLag();
    return std::isinf(time) ? time : time - m_storedTimeLag;
}
