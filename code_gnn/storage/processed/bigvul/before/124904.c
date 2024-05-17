LayoutUnit RenderBox::offsetFromLogicalTopOfFirstPage() const
{
    LayoutState* layoutState = view()->layoutState();
    if (layoutState && !layoutState->isPaginated())
        return 0;

    if (!layoutState && !flowThreadContainingBlock())
        return 0;

    RenderBlock* containerBlock = containingBlock();
    return containerBlock->offsetFromLogicalTopOfFirstPage() + logicalTop();
}
