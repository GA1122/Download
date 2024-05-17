LayoutRect RenderBox::layoutOverflowRectForPropagation(RenderStyle* parentStyle) const
{
    LayoutRect rect = borderBoxRect();
    if (!style()->hasMarginAfterQuirk() && !isSelfCollapsingBlock())
        rect.expand(isHorizontalWritingMode() ? LayoutSize(LayoutUnit(), marginAfter()) : LayoutSize(marginAfter(), LayoutUnit()));

    if (!hasOverflowClip())
        rect.unite(layoutOverflowRect());

    bool hasTransform = hasLayer() && layer()->transform();
    if (isInFlowPositioned() || hasTransform) {
        flipForWritingMode(rect);

        if (hasTransform)
            rect = layer()->currentTransform().mapRect(rect);

        if (isInFlowPositioned())
            rect.move(offsetForInFlowPosition());

        flipForWritingMode(rect);
    }

    if (parentStyle->writingMode() == style()->writingMode())
        return rect;

    if (style()->writingMode() == RightToLeftWritingMode || parentStyle->writingMode() == RightToLeftWritingMode)
        rect.setX(width() - rect.maxX());
    else if (style()->writingMode() == BottomToTopWritingMode || parentStyle->writingMode() == BottomToTopWritingMode)
        rect.setY(height() - rect.maxY());

    return rect;
}
