String WebPageProxy::pageTitle() const
{
    if (!m_mainFrame)
        return String();

    return m_mainFrame->title();
}
