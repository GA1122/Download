void RenderBlock::removePercentHeightDescendantIfNeeded(RenderBox* descendant)
{
    if (!hasPercentHeightContainerMap())
        return;

    if (!hasPercentHeightDescendant(descendant))
        return;

    removePercentHeightDescendant(descendant);
}
