void WebPageProxy::unmarkAllMisspellings()
{
    process()->send(Messages::WebPage::UnmarkAllMisspellings(), m_pageID);
}
