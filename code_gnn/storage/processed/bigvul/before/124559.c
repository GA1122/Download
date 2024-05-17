bool RenderBlock::hasPercentHeightDescendant(RenderBox* descendant)
{
    ASSERT(gPercentHeightContainerMap);
    return gPercentHeightContainerMap->contains(descendant);
}
