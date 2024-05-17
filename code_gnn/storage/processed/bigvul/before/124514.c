static void clipOutPositionedObjects(const PaintInfo* paintInfo, const LayoutPoint& offset, TrackedRendererListHashSet* positionedObjects)
{
    if (!positionedObjects)
        return;

    TrackedRendererListHashSet::const_iterator end = positionedObjects->end();
    for (TrackedRendererListHashSet::const_iterator it = positionedObjects->begin(); it != end; ++it) {
        RenderBox* r = *it;
        paintInfo->context->clipOut(IntRect(offset.x() + r->x(), offset.y() + r->y(), r->width(), r->height()));
    }
}
