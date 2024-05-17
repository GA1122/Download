void DisplayItemList::invalidate(const DisplayItemClientWrapper& client, PaintInvalidationReason paintInvalidationReason, const IntRect& previousPaintInvalidationRect, const IntRect& newPaintInvalidationRect)
{
    invalidateUntracked(client.displayItemClient());
    if (RuntimeEnabledFeatures::slimmingPaintSynchronizedPaintingEnabled()) {
        Invalidation invalidation = { previousPaintInvalidationRect, paintInvalidationReason };
        if (!previousPaintInvalidationRect.isEmpty())
            m_invalidations.append(invalidation);
        if (newPaintInvalidationRect != previousPaintInvalidationRect && !newPaintInvalidationRect.isEmpty()) {
            invalidation.rect = newPaintInvalidationRect;
            m_invalidations.append(invalidation);
        }
    }

    if (RuntimeEnabledFeatures::slimmingPaintV2Enabled() && m_trackedPaintInvalidationObjects)
        m_trackedPaintInvalidationObjects->append(client.debugName());
}
