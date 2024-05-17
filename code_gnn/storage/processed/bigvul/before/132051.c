static void clipOutPositionedObjects(ClipScope& clipScope, const LayoutPoint& offset, TrackedLayoutBoxListHashSet* positionedObjects)
{
    if (!positionedObjects)
        return;

    TrackedLayoutBoxListHashSet::const_iterator end = positionedObjects->end();
    for (TrackedLayoutBoxListHashSet::const_iterator it = positionedObjects->begin(); it != end; ++it) {
        LayoutBox* r = *it;
        clipScope.clip(LayoutRect(flooredIntPoint(r->location() + offset), flooredIntSize(r->size())), SkRegion::kDifference_Op);
    }
}
