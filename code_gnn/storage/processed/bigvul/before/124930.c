int RenderBox::pixelSnappedOffsetWidth() const
{
    return snapSizeToPixel(offsetWidth(), x() + clientLeft());
}
