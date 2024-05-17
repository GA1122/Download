int Document::elapsedTime() const
{
    return static_cast<int>((currentTime() - m_startTime) * 1000);
}
