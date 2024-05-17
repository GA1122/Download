bool WebTransformOperations::isIdentity() const
{
    for (size_t i = 0; i < m_private->operations.size(); ++i) {
        if (!m_private->operations[i].isIdentity())
            return false;
    }
    return true;
}
