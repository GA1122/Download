DisplayItems::iterator DisplayItemList::findOutOfOrderCachedItemForward(const DisplayItem::Id& id, OutOfOrderIndexContext& context)
{
    DisplayItems::iterator currentEnd = m_currentDisplayItems.end();
    for (; context.nextItemToIndex != currentEnd; ++context.nextItemToIndex) {
        const DisplayItem& item = *context.nextItemToIndex;
        ASSERT(item.isValid());
        if (item.isCacheable() && clientCacheIsValid(item.client())) {
            if (id.matches(item))
                return context.nextItemToIndex++;

            addItemToIndexIfNeeded(item, context.nextItemToIndex - m_currentDisplayItems.begin(), context.displayItemIndicesByClient);
        }
    }
    return currentEnd;
}
