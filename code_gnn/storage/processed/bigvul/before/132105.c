bool LayoutBlockFlow::mustSeparateMarginAfterForChild(const LayoutBox& child) const
{
    ASSERT(!child.selfNeedsLayout());
    const ComputedStyle& childStyle = child.styleRef();
    if (!child.isWritingModeRoot())
        return childStyle.marginAfterCollapse() == MSEPARATE;
    if (child.isHorizontalWritingMode() == isHorizontalWritingMode())
        return childStyle.marginBeforeCollapse() == MSEPARATE;

    return false;
}
