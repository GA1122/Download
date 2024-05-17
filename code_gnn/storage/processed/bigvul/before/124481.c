void RenderBlock::addOverflowFromPositionedObjects()
{
    TrackedRendererListHashSet* positionedDescendants = positionedObjects();
    if (!positionedDescendants)
        return;

    RenderBox* positionedObject;
    TrackedRendererListHashSet::iterator end = positionedDescendants->end();
    for (TrackedRendererListHashSet::iterator it = positionedDescendants->begin(); it != end; ++it) {
        positionedObject = *it;

        if (positionedObject->style()->position() != FixedPosition)
            addOverflowFromChild(positionedObject, LayoutSize(positionedObject->x(), positionedObject->y()));
    }
}
