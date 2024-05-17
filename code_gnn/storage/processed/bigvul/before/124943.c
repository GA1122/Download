int RenderBox::scrollHeight() const
{
    if (hasOverflowClip())
        return layer()->scrollableArea()->scrollHeight();
    return snapSizeToPixel(max(clientHeight(), layoutOverflowRect().maxY() - borderTop()), y() + clientTop());
}
