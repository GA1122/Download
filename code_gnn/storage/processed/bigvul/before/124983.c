ItemPosition RenderFlexibleBox::alignmentForChild(RenderBox* child) const
{
    ItemPosition align = resolveAlignment(style(), child->style());

    if (align == ItemPositionBaseline && hasOrthogonalFlow(child))
        align = ItemPositionFlexStart;

    if (style()->flexWrap() == FlexWrapReverse) {
        if (align == ItemPositionFlexStart)
            align = ItemPositionFlexEnd;
        else if (align == ItemPositionFlexEnd)
            align = ItemPositionFlexStart;
    }

    return align;
}
