void LayoutBlockFlow::addChild(LayoutObject* newChild, LayoutObject* beforeChild)
{
    if (LayoutMultiColumnFlowThread* flowThread = multiColumnFlowThread()) {
        if (beforeChild == flowThread)
            beforeChild = flowThread->firstChild();
        ASSERT(!beforeChild || beforeChild->isDescendantOf(flowThread));
        flowThread->addChild(newChild, beforeChild);
        return;
    }
    LayoutBlock::addChild(newChild, beforeChild);
}
