void DisplayItemList::checkUnderInvalidation(DisplayItems::iterator& newIt, DisplayItems::iterator& currentIt)
{
    ASSERT(RuntimeEnabledFeatures::slimmingPaintUnderInvalidationCheckingEnabled());
    ASSERT(newIt->isCached());

    DisplayItem::Type nextItemType = DisplayItem::nonCachedType(newIt->type());
    ++newIt;
    ASSERT(newIt->type() == nextItemType);

    if (newIt->isDrawing()) {
        checkCachedDisplayItemIsUnchanged("", *newIt, *currentIt);
        return;
    }

    ASSERT(newIt->isSubsequence());

#ifndef NDEBUG
    CString messagePrefix = String::format("(In CachedSubsequence of %s)", newIt->clientDebugString().utf8().data()).utf8();
#else
    CString messagePrefix = "(In CachedSubsequence)";
#endif

    DisplayItem::Id endSubsequenceId(newIt->client(), DisplayItem::subsequenceTypeToEndSubsequenceType(newIt->type()), 0);
    while (true) {
        ASSERT(newIt != m_newDisplayItems.end());
        if (newIt->isCached())
            checkUnderInvalidation(newIt, currentIt);
        else
            checkCachedDisplayItemIsUnchanged(messagePrefix.data(), *newIt, *currentIt);

        if (endSubsequenceId.matches(*newIt))
            break;

        ++newIt;
        ++currentIt;
    }
}
