bool WebTransformOperations::matchesTypes(const WebTransformOperations& other) const
{
    if (isIdentity() || other.isIdentity())
        return true;

    if (m_private->operations.size() != other.m_private->operations.size())
        return false;

    for (size_t i = 0; i < m_private->operations.size(); ++i) {
        if (m_private->operations[i].type != other.m_private->operations[i].type
            && !m_private->operations[i].isIdentity()
            && !other.m_private->operations[i].isIdentity())
            return false;
    }

     return true;
 }
