LayoutUnit LayoutBlockFlow::logicalRightSelectionOffset(const LayoutBlock* rootBlock, LayoutUnit position) const
{
    LayoutUnit logicalRight = logicalRightOffsetForLine(position, false);
    if (logicalRight == logicalRightOffsetForContent())
        return LayoutBlock::logicalRightSelectionOffset(rootBlock, position);

    const LayoutBlock* cb = this;
    while (cb != rootBlock) {
        logicalRight += cb->logicalLeft();
        cb = cb->containingBlock();
    }
    return logicalRight;
}
