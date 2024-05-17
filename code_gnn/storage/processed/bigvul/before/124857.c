LayoutUnit RenderBox::containingBlockLogicalHeightForPositioned(const RenderBoxModelObject* containingBlock, bool checkForPerpendicularWritingMode) const
{
    if (checkForPerpendicularWritingMode && containingBlock->isHorizontalWritingMode() != isHorizontalWritingMode())
        return containingBlockLogicalWidthForPositioned(containingBlock, false);

    if (style()->position() == FixedPosition && containingBlock->isRenderView()) {
        const RenderView* view = toRenderView(containingBlock);
        if (FrameView* frameView = view->frameView()) {
            LayoutRect viewportRect = frameView->viewportConstrainedVisibleContentRect();
            return containingBlock->isHorizontalWritingMode() ? viewportRect.height() : viewportRect.width();
        }
    }

    if (containingBlock->isBox()) {
        const RenderBlock* cb = containingBlock->isRenderBlock() ?
            toRenderBlock(containingBlock) : containingBlock->containingBlock();
        return cb->clientLogicalHeight();
    }

    ASSERT(containingBlock->isRenderInline() && containingBlock->isInFlowPositioned());

    const RenderInline* flow = toRenderInline(containingBlock);
    InlineFlowBox* first = flow->firstLineBox();
    InlineFlowBox* last = flow->lastLineBox();

    if (!first || !last)
        return 0;

    LayoutUnit heightResult;
    LayoutRect boundingBox = flow->linesBoundingBox();
    if (containingBlock->isHorizontalWritingMode())
        heightResult = boundingBox.height();
    else
        heightResult = boundingBox.width();
    heightResult -= (containingBlock->borderBefore() + containingBlock->borderAfter());
    return heightResult;
}
