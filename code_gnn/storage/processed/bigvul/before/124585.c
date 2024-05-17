LayoutUnit RenderBlock::logicalLeftSelectionOffset(RenderBlock* rootBlock, LayoutUnit position)
{
    if (rootBlock != this)
        return containingBlock()->logicalLeftSelectionOffset(rootBlock, position + logicalTop());
    return logicalLeftOffsetForContent();
}
