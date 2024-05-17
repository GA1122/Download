void WebPageProxy::hideFindUI()
{
    process()->send(Messages::WebPage::HideFindUI(), m_pageID);
}
