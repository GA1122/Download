RenderObject* RenderBlockFlow::layoutSpecialExcludedChild(bool relayoutChildren, SubtreeLayoutScope& layoutScope)
{
    RenderMultiColumnFlowThread* flowThread = multiColumnFlowThread();
    if (!flowThread)
        return 0;
    setLogicalTopForChild(flowThread, borderBefore() + paddingBefore());
    flowThread->layoutColumns(relayoutChildren, layoutScope);
    determineLogicalLeftPositionForChild(flowThread);
    return flowThread;
}
