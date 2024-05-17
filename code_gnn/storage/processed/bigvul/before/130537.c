DisplayItems::iterator DisplayItemList::findOutOfOrderCachedItem(const DisplayItem::Id& id, OutOfOrderIndexContext& context)
{
    ASSERT(clientCacheIsValid(id.client));

    size_t foundIndex = findMatchingItemFromIndex(id, context.displayItemIndicesByClient, m_currentDisplayItems);
    if (foundIndex != kNotFound)
        return m_currentDisplayItems.begin() + foundIndex;

    return findOutOfOrderCachedItemForward(id, context);
}
