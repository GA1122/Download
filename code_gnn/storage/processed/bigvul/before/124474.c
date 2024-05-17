void RenderBlock::addChildIgnoringContinuation(RenderObject* newChild, RenderObject* beforeChild)
{
    if (!isAnonymousBlock() && firstChild() && (firstChild()->isAnonymousColumnsBlock() || firstChild()->isAnonymousColumnSpanBlock()))
        addChildToAnonymousColumnBlocks(newChild, beforeChild);
    else
        addChildIgnoringAnonymousColumnBlocks(newChild, beforeChild);
}
