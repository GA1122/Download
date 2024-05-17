XSLTProcessor::~XSLTProcessor()
{
    ASSERT(!m_stylesheetRootNode || !m_stylesheet || m_stylesheet->hasOneRef());
}
