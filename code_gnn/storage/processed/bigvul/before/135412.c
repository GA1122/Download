WebTaskRunner* Document::timerTaskRunner() const
{
    if (frame())
        return m_frame->frameScheduler()->timerTaskRunner();
    if (m_importsController)
        return m_importsController->master()->timerTaskRunner();
    if (m_contextDocument)
        return m_contextDocument->timerTaskRunner();
    return Platform::current()->currentThread()->scheduler()->timerTaskRunner();
}
