void DisplayItemList::invalidateAll()
{
    ASSERT(m_newDisplayItems.isEmpty());
    m_currentDisplayItems.clear();
    m_validlyCachedClients.clear();
    m_validlyCachedClientsDirty = false;

    if (RuntimeEnabledFeatures::slimmingPaintV2Enabled() && m_trackedPaintInvalidationObjects)
        m_trackedPaintInvalidationObjects->append("##ALL##");
}
