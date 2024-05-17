LayoutUnit RenderBlockFlow::logicalLeftSelectionOffset(RenderBlock* rootBlock, LayoutUnit position)
{
    LayoutUnit logicalLeft = logicalLeftOffsetForLine(position, false);
    if (logicalLeft == logicalLeftOffsetForContent())
        return RenderBlock::logicalLeftSelectionOffset(rootBlock, position);

    RenderBlock* cb = this;
    while (cb != rootBlock) {
        logicalLeft += cb->logicalLeft();
        cb = cb->containingBlock();
    }
    return logicalLeft;
}
