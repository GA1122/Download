bool AffineTransform::isInvertible() const
{
    return det() != 0.0;
}
