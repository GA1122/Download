TransformationMatrix AffineTransform::toTransformationMatrix() const
{
    return TransformationMatrix(m_transform[0], m_transform[1], m_transform[2],
                                m_transform[3], m_transform[4], m_transform[5]);
}
