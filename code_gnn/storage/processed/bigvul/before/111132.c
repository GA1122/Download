static inline int distanceBetweenPoints(IntPoint p1, IntPoint p2)
{
    double dx = static_cast<double>(p1.x() - p2.x());
    double dy = static_cast<double>(p1.y() - p2.y());
    return sqrt((dx * dx) + (dy * dy));
}
