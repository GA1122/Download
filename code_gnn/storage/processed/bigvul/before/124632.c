void RenderBlock::removePositionedObject(RenderBox* o)
{
    removeFromTrackedRendererMaps(o, gPositionedDescendantsMap, gPositionedContainerMap);
}
