LayoutRect RenderBox::visualOverflowRectForPropagation(RenderStyle* parentStyle) const
{
    LayoutRect rect = visualOverflowRect();
    if (parentStyle->writingMode() == style()->writingMode())
        return rect;

    if (style()->writingMode() == RightToLeftWritingMode || parentStyle->writingMode() == RightToLeftWritingMode)
        rect.setX(width() - rect.maxX());
    else if (style()->writingMode() == BottomToTopWritingMode || parentStyle->writingMode() == BottomToTopWritingMode)
        rect.setY(height() - rect.maxY());

    return rect;
}
