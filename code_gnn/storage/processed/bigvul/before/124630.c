void RenderBlock::removePercentHeightDescendant(RenderBox* descendant)
{
    removeFromTrackedRendererMaps(descendant, gPercentHeightDescendantsMap, gPercentHeightContainerMap);
}
