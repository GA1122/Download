LayoutUnit RenderFlexibleBox::marginBoxAscentForChild(RenderBox* child)
{
    LayoutUnit ascent = child->firstLineBoxBaseline();
    if (ascent == -1)
        ascent = crossAxisExtentForChild(child);
    return ascent + flowAwareMarginBeforeForChild(child);
}
