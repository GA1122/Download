void DisplayItemList::commitNewDisplayItemsAndAppendToWebDisplayItemList(WebDisplayItemList* list)
{
    commitNewDisplayItems();
    appendToWebDisplayItemList(list);
}
