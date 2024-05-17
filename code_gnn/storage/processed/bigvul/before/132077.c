bool LayoutBlockFlow::isPagedOverflow(const ComputedStyle& style)
{
    return style.isOverflowPaged() && node() != document().viewportDefiningElement();
}
