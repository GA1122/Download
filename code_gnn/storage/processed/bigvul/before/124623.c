TrackedRendererListHashSet* RenderBlock::positionedObjects() const
{
    if (gPositionedDescendantsMap)
        return gPositionedDescendantsMap->get(this);
    return 0;
}
