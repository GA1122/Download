LayoutUnit RenderBlock::collapsedMarginAfterForChild(const  RenderBox* child) const
{
    if (!child->isWritingModeRoot())
        return child->collapsedMarginAfter();

    if (child->isHorizontalWritingMode() == isHorizontalWritingMode())
        return child->collapsedMarginBefore();

    return marginAfterForChild(child);
}
