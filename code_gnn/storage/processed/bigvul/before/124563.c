RenderObject* RenderBlock::hoverAncestor() const
{
    return isAnonymousBlockContinuation() ? continuation() : RenderBox::hoverAncestor();
}
