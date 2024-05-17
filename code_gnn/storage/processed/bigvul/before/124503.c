RenderBlock* RenderBlock::blockElementContinuation() const
{
    RenderBoxModelObject* currentContinuation = continuation();
    if (!currentContinuation || currentContinuation->isInline())
        return 0;
    RenderBlock* nextContinuation = toRenderBlock(currentContinuation);
    if (nextContinuation->isAnonymousBlock())
        return nextContinuation->blockElementContinuation();
    return nextContinuation;
}
