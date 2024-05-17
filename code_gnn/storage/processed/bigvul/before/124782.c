bool RenderBlockFlow::updateLogicalWidthAndColumnWidth()
{
    bool relayoutChildren = RenderBlock::updateLogicalWidthAndColumnWidth();
    if (RenderMultiColumnFlowThread* flowThread = multiColumnFlowThread()) {
        if (flowThread->computeColumnCountAndWidth())
            return true;
    }
    return relayoutChildren;
}
