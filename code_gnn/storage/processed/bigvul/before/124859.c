LayoutUnit RenderBox::containingBlockLogicalWidthForPositioned(const RenderBoxModelObject* containingBlock, bool checkForPerpendicularWritingMode) const
{
    if (checkForPerpendicularWritingMode && containingBlock->isHorizontalWritingMode() != isHorizontalWritingMode())
        return containingBlockLogicalHeightForPositioned(containingBlock, false);

    if (style()->position() == FixedPosition && containingBlock->isRenderView()) {
        const RenderView* view = toRenderView(containingBlock);
        if (FrameView* frameView = view->frameView()) {
            LayoutRect viewportRect = frameView->viewportConstrainedVisibleContentRect();
            return containingBlock->isHorizontalWritingMode() ? viewportRect.width() : viewportRect.height();
        }
    }

    if (containingBlock->isBox())
        return toRenderBox(containingBlock)->clientLogicalWidth();

    ASSERT(containingBlock->isRenderInline() && containingBlock->isInFlowPositioned());

    const RenderInline* flow = toRenderInline(containingBlock);
    InlineFlowBox* first = flow->firstLineBox();
    InlineFlowBox* last = flow->lastLineBox();

    if (!first || !last)
        return 0;

    LayoutUnit fromLeft;
    LayoutUnit fromRight;
    if (containingBlock->style()->isLeftToRightDirection()) {
        fromLeft = first->logicalLeft() + first->borderLogicalLeft();
        fromRight = last->logicalLeft() + last->logicalWidth() - last->borderLogicalRight();
    } else {
        fromRight = first->logicalLeft() + first->logicalWidth() - first->borderLogicalRight();
        fromLeft = last->logicalLeft() + last->borderLogicalLeft();
    }

    return max<LayoutUnit>(0, fromRight - fromLeft);
}
