int RenderBox::pixelSnappedClientWidth() const
{
    return snapSizeToPixel(clientWidth(), x() + clientLeft());
}
