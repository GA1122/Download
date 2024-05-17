int RenderBox::pixelSnappedClientHeight() const
{
    return snapSizeToPixel(clientHeight(), y() + clientTop());
}
