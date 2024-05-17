AffineTransform& AffineTransform::rotateRadians(double a)
{
    double cosAngle = cos(a);
    double sinAngle = sin(a);
    AffineTransform rot(cosAngle, sinAngle, -sinAngle, cosAngle, 0, 0);

    multiply(rot);
    return *this;
}
