WebTaskRunner* Document::loadingTaskRunner() const
{
    if (frame())
        return frame()->frameScheduler()->loadingTaskRunner();
    if (m_importsController)
        return m_importsController->master()->loadingTaskRunner();
    if (m_contextDocument)
        return m_contextDocument->loadingTaskRunner();
    return Platform::current()->currentThread()->scheduler()->loadingTaskRunner();
}
