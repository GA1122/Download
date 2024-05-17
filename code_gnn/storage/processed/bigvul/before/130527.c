void DisplayItemList::checkCachedDisplayItemIsUnchanged(const char* messagePrefix, const DisplayItem& newItem, const DisplayItem& oldItem)
{
    ASSERT(RuntimeEnabledFeatures::slimmingPaintUnderInvalidationCheckingEnabled());
    ASSERT(!newItem.isCached());
    ASSERT(!oldItem.isCached());

    if (newItem.skippedCache()) {
        showUnderInvalidationError(messagePrefix, "ERROR: under-invalidation: skipped-cache in cached subsequence", &newItem, &oldItem);
        ASSERT_NOT_REACHED();
    }

    if (newItem.isCacheable() && !m_validlyCachedClients.contains(newItem.client())) {
        showUnderInvalidationError(messagePrefix, "ERROR: under-invalidation: invalidated in cached subsequence", &newItem, &oldItem);
        ASSERT_NOT_REACHED();
    }

    if (newItem.equals(oldItem))
        return;

    showUnderInvalidationError(messagePrefix, "ERROR: under-invalidation: display item changed", &newItem, &oldItem);

#ifndef NDEBUG
    if (newItem.isDrawing()) {
        RefPtr<const SkPicture> newPicture = static_cast<const DrawingDisplayItem&>(newItem).picture();
        RefPtr<const SkPicture> oldPicture = static_cast<const DrawingDisplayItem&>(oldItem).picture();
        String oldPictureDebugString = oldPicture ? pictureAsDebugString(oldPicture.get()) : "None";
        String newPictureDebugString = newPicture ? pictureAsDebugString(newPicture.get()) : "None";
        WTFLogAlways("old picture:\n%s\n", oldPictureDebugString.utf8().data());
        WTFLogAlways("new picture:\n%s\n", newPictureDebugString.utf8().data());
    }
#endif  

    ASSERT_NOT_REACHED();
}
