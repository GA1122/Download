void DisplayItemList::appendToWebDisplayItemList(WebDisplayItemList* list)
{
    for (const DisplayItem& item : m_currentDisplayItems)
        item.appendToWebDisplayItemList(list);
}
