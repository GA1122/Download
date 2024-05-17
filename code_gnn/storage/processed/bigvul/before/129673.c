AffineTransform& AffineTransform::skewY(double angle)
{
    return shear(0, tan(deg2rad(angle)));
}
