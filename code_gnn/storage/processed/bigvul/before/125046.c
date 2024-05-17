void RenderFlexibleBox::prepareChildForPositionedLayout(RenderBox* child, LayoutUnit mainAxisOffset, LayoutUnit crossAxisOffset, PositionedLayoutMode layoutMode)
{
    ASSERT(child->isOutOfFlowPositioned());
    child->containingBlock()->insertPositionedObject(child);
    RenderLayer* childLayer = child->layer();
    LayoutUnit inlinePosition = isColumnFlow() ? crossAxisOffset : mainAxisOffset;
    if (layoutMode == FlipForRowReverse && style()->flexDirection() == FlowRowReverse)
        inlinePosition = mainAxisExtent() - mainAxisOffset;
    childLayer->setStaticInlinePosition(inlinePosition);

    LayoutUnit staticBlockPosition = isColumnFlow() ? mainAxisOffset : crossAxisOffset;
    if (childLayer->staticBlockPosition() != staticBlockPosition) {
        childLayer->setStaticBlockPosition(staticBlockPosition);
        if (child->style()->hasStaticBlockPosition(style()->isHorizontalWritingMode()))
            child->setChildNeedsLayout(MarkOnlyThis);
    }
}
