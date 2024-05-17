String PopupContainer::getSelectedItemToolTip()
{
    return listBox()->m_popupClient->itemToolTip(listBox()->m_selectedIndex);
}
