void WebPage::popupClosed()
{
    ASSERT(d->m_selectPopup);
    d->m_selectPopup = 0;
}
