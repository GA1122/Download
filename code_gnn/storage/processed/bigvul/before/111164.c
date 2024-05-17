void WebPagePrivate::frameUnloaded(const Frame* frame)
{
    m_inputHandler->frameUnloaded(frame);
    m_inPageSearchManager->frameUnloaded(frame);
}
