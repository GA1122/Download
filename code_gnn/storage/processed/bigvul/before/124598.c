Node* RenderBlock::nodeForHitTest() const
{
    return isAnonymousBlockContinuation() ? continuation()->node() : node();
}
