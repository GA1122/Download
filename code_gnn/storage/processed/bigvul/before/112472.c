PassRefPtr<NodeList> Document::getItems(const String& typeNames)
{
    String localTypeNames = typeNames.isNull() ? MicroDataItemList::undefinedItemType() : typeNames;

    return ensureRareData()->ensureNodeLists()->addCacheWithName<MicroDataItemList>(this, MicroDataItemListType, localTypeNames);
}
