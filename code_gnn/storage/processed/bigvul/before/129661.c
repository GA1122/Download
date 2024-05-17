AffineTransform& AffineTransform::preMultiply(const AffineTransform& other)
{
    if (other.isIdentityOrTranslation()) {
        if (other.m_transform[4] || other.m_transform[5]) {
            m_transform[4] += other.m_transform[4];
            m_transform[5] += other.m_transform[5];
        }
        return *this;
    }

    AffineTransform trans;
    doMultiply(other, *this, &trans);
    setMatrix(trans.m_transform);

    return *this;
}
