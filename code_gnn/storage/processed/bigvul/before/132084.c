LayoutObject* LayoutBlockFlow::layoutSpecialExcludedChild(bool relayoutChildren, SubtreeLayoutScope& layoutScope)
{
    LayoutMultiColumnFlowThread* flowThread = multiColumnFlowThread();
    if (!flowThread)
        return nullptr;
    setLogicalTopForChild(*flowThread, borderBefore() + paddingBefore());
    flowThread->layoutColumns(relayoutChildren, layoutScope);
    determineLogicalLeftPositionForChild(*flowThread);
    return flowThread;
}
