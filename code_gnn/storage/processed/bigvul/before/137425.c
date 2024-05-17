void NavigatorServiceWorker::willDetachGlobalObjectFromFrame()
{
    if (m_serviceWorker) {
        m_serviceWorker->willBeDetachedFromFrame();
        m_serviceWorker = nullptr;
    }
}
