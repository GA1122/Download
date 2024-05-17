 PlatformWebView::~PlatformWebView()
{
    delete m_window;
    if (m_modalEventLoop)
        m_modalEventLoop->exit();
}
