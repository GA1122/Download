IntSize AffineTransform::mapSize(const IntSize& size) const
{
    double width2 = size.width() * xScale();
    double height2 = size.height() * yScale();

    return IntSize(lround(width2), lround(height2));
}
