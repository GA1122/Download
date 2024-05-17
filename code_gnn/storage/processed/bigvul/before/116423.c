WebTransformationMatrix WebTransformOperations::apply() const
{
    WebTransformationMatrix toReturn;
    for (size_t i = 0; i < m_private->operations.size(); ++i)
        toReturn.multiply(m_private->operations[i].matrix);
    return toReturn;
}
