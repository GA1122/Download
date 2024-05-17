void RenderBlock::addChild(RenderObject* newChild, RenderObject* beforeChild)
{
    if (continuation() && !isAnonymousBlock())
        addChildToContinuation(newChild, beforeChild);
    else
        addChildIgnoringContinuation(newChild, beforeChild);
}
