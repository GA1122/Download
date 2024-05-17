RenderStyle* RenderBlock::outlineStyleForRepaint() const
{
    return isAnonymousBlockContinuation() ? continuation()->style() : style();
}
