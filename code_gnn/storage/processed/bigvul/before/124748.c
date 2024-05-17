bool RenderBlockFlow::mustSeparateMarginBeforeForChild(const RenderBox* child) const
{
    ASSERT(!child->selfNeedsLayout());
    const RenderStyle* childStyle = child->style();
    if (!child->isWritingModeRoot())
        return childStyle->marginBeforeCollapse() == MSEPARATE;
    if (child->isHorizontalWritingMode() == isHorizontalWritingMode())
        return childStyle->marginAfterCollapse() == MSEPARATE;

    return false;
}
