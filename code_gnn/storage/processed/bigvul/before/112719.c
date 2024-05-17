DocumentLoader::~DocumentLoader()
{
    ASSERT(!m_frame || frameLoader()->activeDocumentLoader() != this || !isLoading());
    if (m_iconLoadDecisionCallback)
        m_iconLoadDecisionCallback->invalidate();
    if (m_iconDataCallback)
        m_iconDataCallback->invalidate();
    m_cachedResourceLoader->clearDocumentLoader();
    
    if (m_mainResource) {
        m_mainResource->removeClient(this);
        m_mainResource = 0;
    }
}
