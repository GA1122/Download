void WebPagePrivate::destroyCompositor()
{
    m_compositor->detach();
    m_compositor.clear();
    m_ownedContext.clear();
}
