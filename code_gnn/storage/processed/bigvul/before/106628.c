void WebPageProxy::setInitialFocus(bool forward)
{
    if (!isValid())
        return;
    process()->send(Messages::WebPage::SetInitialFocus(forward), m_pageID);
}
