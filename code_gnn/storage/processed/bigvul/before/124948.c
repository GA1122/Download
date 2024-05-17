int RenderBox::scrollWidth() const
{
    if (hasOverflowClip())
        return layer()->scrollableArea()->scrollWidth();
    if (style()->isLeftToRightDirection())
        return snapSizeToPixel(max(clientWidth(), layoutOverflowRect().maxX() - borderLeft()), x() + clientLeft());
    return clientWidth() - min<LayoutUnit>(0, layoutOverflowRect().x() - borderLeft());
}
