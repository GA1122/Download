DEFINE_TRACE(NodeIterator)
{
    visitor->trace(m_referenceNode);
    visitor->trace(m_candidateNode);
    NodeIteratorBase::trace(visitor);
}
