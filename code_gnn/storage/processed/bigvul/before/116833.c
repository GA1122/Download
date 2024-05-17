inline bool PopupContainer::isRTL() const
{
    return m_listBox->m_popupClient->menuStyle().textDirection() == RTL;
}
