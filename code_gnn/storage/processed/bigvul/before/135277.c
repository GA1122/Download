void Document::initSecurityContext()
{
    initSecurityContext(DocumentInit(m_url, m_frame, contextDocument(), m_importsController));
}
