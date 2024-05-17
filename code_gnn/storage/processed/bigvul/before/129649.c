bool AffineTransform::isIdentity() const
{
    return (m_transform[0] == 1 && m_transform[1] == 0
         && m_transform[2] == 0 && m_transform[3] == 1
          && m_transform[4] == 0 && m_transform[5] == 0);
 }
