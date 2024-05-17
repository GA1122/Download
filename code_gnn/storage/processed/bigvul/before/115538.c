void TestController::runUntil(bool& done, TimeoutDuration timeoutDuration)
{
    double timeout = m_noTimeout;
    if (!m_forceNoTimeout) {
        switch (timeoutDuration) {
        case ShortTimeout:
            timeout = m_shortTimeout;
            break;
        case LongTimeout:
            timeout = m_longTimeout;
            break;
        case CustomTimeout:
            timeout = m_timeout;
            break;
        case NoTimeout:
        default:
            timeout = m_noTimeout;
            break;
        }
    }

    platformRunUntil(done, timeout);
}
