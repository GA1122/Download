bool RenderBlockFlow::mustDiscardMarginBeforeForChild(const RenderBox* child) const
{
    ASSERT(!child->selfNeedsLayout());
    if (!child->isWritingModeRoot())
        return child->isRenderBlockFlow() ? toRenderBlockFlow(child)->mustDiscardMarginBefore() : (child->style()->marginBeforeCollapse() == MDISCARD);
    if (child->isHorizontalWritingMode() == isHorizontalWritingMode())
        return child->isRenderBlockFlow() ? toRenderBlockFlow(child)->mustDiscardMarginAfter() : (child->style()->marginAfterCollapse() == MDISCARD);

    return false;
}
