void PopupContainer::notifyPopupHidden()
{
    if (!m_popupOpen)
        return;
    m_popupOpen = false;
    chromeClientChromium()->popupClosed(this);
}
