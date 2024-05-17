void Document::didAccessStyleResolver()
{
    static const int timeBeforeThrowingAwayStyleResolverAfterLastUseInSeconds = 60;
    static const int holdOffTimeBeforeReschedulingTimerInSeconds = 5;

    double currentTime = WTF::currentTime();

    if (currentTime > m_lastStyleResolverAccessTime + holdOffTimeBeforeReschedulingTimerInSeconds) {
        m_styleResolverThrowawayTimer.startOneShot(timeBeforeThrowingAwayStyleResolverAfterLastUseInSeconds);
        m_lastStyleResolverAccessTime = currentTime;
    }
}
