DEFINE_TRACE(Frame)
{
    visitor->trace(m_treeNode);
    visitor->trace(m_host);
    visitor->trace(m_owner);
    visitor->trace(m_client);
}
