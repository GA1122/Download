RenderBlock::~RenderBlock()
{
    if (hasColumns())
        gColumnInfoMap->take(this);
    if (gPercentHeightDescendantsMap)
        removeBlockFromDescendantAndContainerMaps(this, gPercentHeightDescendantsMap, gPercentHeightContainerMap);
    if (gPositionedDescendantsMap)
        removeBlockFromDescendantAndContainerMaps(this, gPositionedDescendantsMap, gPositionedContainerMap);
}
