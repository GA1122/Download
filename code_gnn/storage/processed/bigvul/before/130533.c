const DisplayItems& DisplayItemList::displayItems() const
{
    ASSERT(m_newDisplayItems.isEmpty());
     return m_currentDisplayItems;
 }
