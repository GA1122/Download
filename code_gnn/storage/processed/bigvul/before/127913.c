void DocumentThreadableLoader::overrideTimeout(unsigned long timeoutMilliseconds)
{
    ASSERT(m_async);

    if (m_requestStartedSeconds <= 0.0)
        return;

    m_timeoutTimer.stop();
    if (timeoutMilliseconds) {
        double elapsedTime = monotonicallyIncreasingTime() - m_requestStartedSeconds;
        double nextFire = timeoutMilliseconds / 1000.0;
        double resolvedTime = std::max(nextFire - elapsedTime, 0.0);
        m_timeoutTimer.startOneShot(resolvedTime, BLINK_FROM_HERE);
    }
}
