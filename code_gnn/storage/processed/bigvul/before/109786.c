void Document::resetLastHandledUserGestureTimestamp()
{
    m_lastHandledUserGestureTimestamp = currentTime();
}
