bool WebPagePrivate::createCompositor()
{
    if (!m_client->window())
        return false;

    m_ownedContext = GLES2Context::create(this);
    m_compositor = WebPageCompositorPrivate::create(this, 0);
    m_compositor->setContext(m_ownedContext.get());

    m_compositor->setBackgroundColor(m_webSettings->backgroundColor());

    return true;
}
