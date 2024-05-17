void RenderBox::positionLineBox(InlineBox* box)
{
    if (isOutOfFlowPositioned()) {
        bool wasInline = style()->isOriginalDisplayInlineType();
        if (wasInline) {
            RootInlineBox& root = box->root();
            root.block().setStaticInlinePositionForChild(this, root.lineTopWithLeading(), LayoutUnit::fromFloatRound(box->logicalLeft()));
            if (style()->hasStaticInlinePosition(box->isHorizontal()))
                setChildNeedsLayout(MarkOnlyThis);  
        } else {
            layer()->setStaticBlockPosition(box->logicalTop());
            if (style()->hasStaticBlockPosition(box->isHorizontal()))
                setChildNeedsLayout(MarkOnlyThis);  
        }

        box->remove();
        box->destroy();
    } else if (isReplaced()) {
        setLocation(roundedLayoutPoint(box->topLeft()));
        setInlineBoxWrapper(box);
    }
}
