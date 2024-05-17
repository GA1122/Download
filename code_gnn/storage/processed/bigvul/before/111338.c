void WebPagePrivate::setCompositorHelper(PassRefPtr<WebPageCompositorPrivate> compositor)
{
    using namespace BlackBerry::Platform;

    ASSERT(userInterfaceThreadMessageClient()->isCurrentThread());

    m_compositor = compositor;
    if (m_compositor) {
        m_compositor->setPage(this);

        m_compositor->setBackgroundColor(m_webSettings->backgroundColor());
    }

    m_ownedContext.clear();
}
