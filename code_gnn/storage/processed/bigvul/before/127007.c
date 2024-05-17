void ChromeClientImpl::closePagePopup(PagePopup* popup)
{
    ASSERT(m_pagePopupDriver);
    m_pagePopupDriver->closePagePopup(popup);
}
