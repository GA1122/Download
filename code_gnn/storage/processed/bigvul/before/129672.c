AffineTransform& AffineTransform::skewX(double angle)
{
    return shear(tan(deg2rad(angle)), 0);
}
