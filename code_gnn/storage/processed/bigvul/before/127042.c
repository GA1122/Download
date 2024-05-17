PagePopup* ChromeClientImpl::openPagePopup(PagePopupClient* client, const IntRect& originBoundsInRootView)
{
    ASSERT(m_pagePopupDriver);
    return m_pagePopupDriver->openPagePopup(client, originBoundsInRootView);
}
