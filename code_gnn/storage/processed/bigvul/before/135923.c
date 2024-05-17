DEFINE_TRACE(ContainerNode)
{
    visitor->trace(m_firstChild);
    visitor->trace(m_lastChild);
    Node::trace(visitor);
}
