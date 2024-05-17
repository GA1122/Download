LayoutUnit RenderBlock::offsetFromLogicalTopOfFirstPage() const
{
    LayoutState* layoutState = view()->layoutState();
    if (layoutState && !layoutState->isPaginated())
        return 0;

    RenderFlowThread* flowThread = flowThreadContainingBlock();
    if (flowThread)
        return flowThread->offsetFromLogicalTopOfFirstRegion(this);

    if (layoutState) {
        ASSERT(layoutState->renderer() == this);

        LayoutSize offsetDelta = layoutState->layoutOffset() - layoutState->pageOffset();
        return isHorizontalWritingMode() ? offsetDelta.height() : offsetDelta.width();
    }

    ASSERT_NOT_REACHED();
    return 0;
}
