void WebPageProxy::reload(bool reloadFromOrigin)
{
    if (m_backForwardList->currentItem())
        setPendingAPIRequestURL(m_backForwardList->currentItem()->url());

    if (!isValid()) {
        reattachToWebProcessWithItem(m_backForwardList->currentItem());
        return;
    }

    process()->send(Messages::WebPage::Reload(reloadFromOrigin), m_pageID);
}
