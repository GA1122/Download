bool LayoutBlockFlow::updateLogicalWidthAndColumnWidth()
{
    bool relayoutChildren = LayoutBlock::updateLogicalWidthAndColumnWidth();
    if (LayoutMultiColumnFlowThread* flowThread = multiColumnFlowThread()) {
        if (flowThread->needsNewWidth())
            return true;
    }
    return relayoutChildren;
}
