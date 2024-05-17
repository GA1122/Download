void RenderBlock::childBecameNonInline(RenderObject*)
{
    makeChildrenNonInline();
    if (isAnonymousBlock() && parent() && parent()->isRenderBlock())
        toRenderBlock(parent())->removeLeftoverAnonymousBlock(this);
}
