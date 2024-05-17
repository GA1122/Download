void DisplayItemList::checkNoRemainingCachedDisplayItems()
{
    ASSERT(RuntimeEnabledFeatures::slimmingPaintUnderInvalidationCheckingEnabled());

    for (const auto& displayItem : m_currentDisplayItems) {
        if (!displayItem.isValid() || !displayItem.isCacheable() || !clientCacheIsValid(displayItem.client()))
            continue;
        showUnderInvalidationError("", "May be under-invalidation: no new display item", nullptr, &displayItem);
    }
}
