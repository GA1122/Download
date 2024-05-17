void WebPageProxy::stopLoading()
{
    if (!isValid())
        return;
    process()->send(Messages::WebPage::StopLoading(), m_pageID);
}
