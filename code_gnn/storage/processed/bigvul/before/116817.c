PopupContainer::PopupContainer(PopupMenuClient* client,
                               PopupType popupType,
                               const PopupContainerSettings& settings)
    : m_listBox(PopupListBox::create(client, settings))
    , m_settings(settings)
    , m_popupType(popupType)
    , m_popupOpen(false)
{
    setScrollbarModes(ScrollbarAlwaysOff, ScrollbarAlwaysOff);
}
