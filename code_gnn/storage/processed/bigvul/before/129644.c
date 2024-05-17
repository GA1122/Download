 double AffineTransform::det() const
{
    return m_transform[0] * m_transform[3] - m_transform[1] * m_transform[2];
}
