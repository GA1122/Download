int RenderBox::pixelSnappedOffsetHeight() const
{
    return snapSizeToPixel(offsetHeight(), y() + clientTop());
}
