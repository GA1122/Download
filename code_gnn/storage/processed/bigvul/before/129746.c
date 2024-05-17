bool ResourceFetcher::shouldLoadNewResource(Resource::Type type) const
{
    if (!frame())
        return false;
    if (!m_documentLoader)
        return true;
    if (type == Resource::MainResource)
        return m_documentLoader == frame()->loader().provisionalDocumentLoader();
    return m_documentLoader == frame()->loader().documentLoader();
}
