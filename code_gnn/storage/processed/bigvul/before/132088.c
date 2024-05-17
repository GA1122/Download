LayoutUnit LayoutBlockFlow::logicalLeftSelectionOffset(const LayoutBlock* rootBlock, LayoutUnit position) const
{
    LayoutUnit logicalLeft = logicalLeftOffsetForLine(position, false);
    if (logicalLeft == logicalLeftOffsetForContent())
        return LayoutBlock::logicalLeftSelectionOffset(rootBlock, position);

    const LayoutBlock* cb = this;
    while (cb != rootBlock) {
        logicalLeft += cb->logicalLeft();
        cb = cb->containingBlock();
    }
    return logicalLeft;
}
