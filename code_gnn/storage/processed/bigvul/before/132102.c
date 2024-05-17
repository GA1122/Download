bool LayoutBlockFlow::mustDiscardMarginAfterForChild(const LayoutBox& child) const
{
    ASSERT(!child.selfNeedsLayout());
    if (!child.isWritingModeRoot())
        return child.isLayoutBlockFlow() ? toLayoutBlockFlow(&child)->mustDiscardMarginAfter() : (child.style()->marginAfterCollapse() == MDISCARD);
    if (child.isHorizontalWritingMode() == isHorizontalWritingMode())
        return child.isLayoutBlockFlow() ? toLayoutBlockFlow(&child)->mustDiscardMarginBefore() : (child.style()->marginBeforeCollapse() == MDISCARD);

    return false;
}
