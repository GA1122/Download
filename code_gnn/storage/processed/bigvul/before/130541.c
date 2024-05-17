void DisplayItemList::invalidatePaintOffset(const DisplayItemClientWrapper& client)
{
    ASSERT(RuntimeEnabledFeatures::slimmingPaintOffsetCachingEnabled());

    updateValidlyCachedClientsIfNeeded();
    m_validlyCachedClients.remove(client.displayItemClient());

    if (RuntimeEnabledFeatures::slimmingPaintV2Enabled() && m_trackedPaintInvalidationObjects)
        m_trackedPaintInvalidationObjects->append(client.debugName());

#if ENABLE(ASSERT)
    m_clientsWithPaintOffsetInvalidations.add(client.displayItemClient());

    for (const auto& item : m_newDisplayItems)
        ASSERT(!item.isCached() || item.client() != client.displayItemClient());
#endif
}
