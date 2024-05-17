bool DisplayItemList::paintOffsetWasInvalidated(DisplayItemClient client) const
{
    ASSERT(RuntimeEnabledFeatures::slimmingPaintOffsetCachingEnabled());
    return m_clientsWithPaintOffsetInvalidations.contains(client);
}
