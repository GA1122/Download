IntPoint AffineTransform::mapPoint(const IntPoint& point) const
{
    double x2, y2;
    map(point.x(), point.y(), x2, y2);

    return IntPoint(lround(x2), lround(y2));
}
