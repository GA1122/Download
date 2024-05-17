AffineTransform& AffineTransform::rotateFromVector(double x, double y)
{
    return rotateRadians(atan2(y, x));
}
