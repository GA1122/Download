RenderBox* RenderBlock::createAnonymousBoxWithSameTypeAs(const RenderObject* parent) const
{
    if (isAnonymousColumnsBlock())
        return createAnonymousColumnsWithParentRenderer(parent);
    if (isAnonymousColumnSpanBlock())
        return createAnonymousColumnSpanWithParentRenderer(parent);
    return createAnonymousWithParentRendererAndDisplay(parent, style()->display());
}
