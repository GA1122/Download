void WebPage::popupOpened(PagePopupBlackBerry* webPopup)
{
    ASSERT(!d->m_selectPopup);
    d->m_selectPopup = webPopup;
}
