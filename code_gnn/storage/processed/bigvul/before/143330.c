ALWAYS_INLINE void WebFrame::clearWeakFramesImpl(VisitorDispatcher visitor)
{
    if (!isFrameAlive(m_opener))
        m_opener = nullptr;
}
