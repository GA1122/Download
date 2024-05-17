AffineTransform& AffineTransform::multiply(const AffineTransform& other)
{
    if (other.isIdentityOrTranslation()) {
        if (other.m_transform[4] || other.m_transform[5])
            translate(other.m_transform[4], other.m_transform[5]);
        return *this;
    }

    AffineTransform trans;
    doMultiply(*this, other, &trans);
    setMatrix(trans.m_transform);

    return *this;
}
