void RenderBlock::checkPositionedObjectsNeedLayout()
{
    if (!gPositionedDescendantsMap)
        return;

    if (TrackedRendererListHashSet* positionedDescendantSet = positionedObjects()) {
        TrackedRendererListHashSet::const_iterator end = positionedDescendantSet->end();
        for (TrackedRendererListHashSet::const_iterator it = positionedDescendantSet->begin(); it != end; ++it) {
            RenderBox* currBox = *it;
            ASSERT(!currBox->needsLayout());
        }
    }
}
