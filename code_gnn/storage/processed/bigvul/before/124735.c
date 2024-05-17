LayoutUnit RenderBlockFlow::logicalRightSelectionOffset(RenderBlock* rootBlock, LayoutUnit position)
{
    LayoutUnit logicalRight = logicalRightOffsetForLine(position, false);
    if (logicalRight == logicalRightOffsetForContent())
        return RenderBlock::logicalRightSelectionOffset(rootBlock, position);

    RenderBlock* cb = this;
    while (cb != rootBlock) {
        logicalRight += cb->logicalLeft();
        cb = cb->containingBlock();
    }
    return logicalRight;
}
