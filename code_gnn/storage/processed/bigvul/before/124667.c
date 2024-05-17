void RenderBlockFlow::addChild(RenderObject* newChild, RenderObject* beforeChild)
{
    if (RenderMultiColumnFlowThread* flowThread = multiColumnFlowThread())
        return flowThread->addChild(newChild, beforeChild);
    RenderBlock::addChild(newChild, beforeChild);
}
