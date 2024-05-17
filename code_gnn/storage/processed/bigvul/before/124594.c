void RenderBlock::markPositionedObjectsForLayout()
{
    TrackedRendererListHashSet* positionedDescendants = positionedObjects();
    if (positionedDescendants) {
        RenderBox* r;
        TrackedRendererListHashSet::iterator end = positionedDescendants->end();
        for (TrackedRendererListHashSet::iterator it = positionedDescendants->begin(); it != end; ++it) {
            r = *it;
            r->setChildNeedsLayout();
        }
    }
}
