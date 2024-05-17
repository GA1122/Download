bool RenderBlockFlow::mustSeparateMarginAfterForChild(const RenderBox* child) const
{
    ASSERT(!child->selfNeedsLayout());
    const RenderStyle* childStyle = child->style();
    if (!child->isWritingModeRoot())
        return childStyle->marginAfterCollapse() == MSEPARATE;
    if (child->isHorizontalWritingMode() == isHorizontalWritingMode())
        return childStyle->marginBeforeCollapse() == MSEPARATE;

    return false;
}
