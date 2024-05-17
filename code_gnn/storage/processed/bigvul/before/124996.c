LayoutUnit RenderFlexibleBox::computeMainAxisExtentForChild(RenderBox* child, SizeType sizeType, const Length& size)
{
    if (isColumnFlow()) {
        if (size.isIntrinsic())
            child->layoutIfNeeded();
        return child->computeContentLogicalHeight(size, child->logicalHeight() - child->borderAndPaddingLogicalHeight());
    }
    return child->computeLogicalWidthUsing(sizeType, size, contentLogicalWidth(), this) - child->borderAndPaddingLogicalWidth();
}
