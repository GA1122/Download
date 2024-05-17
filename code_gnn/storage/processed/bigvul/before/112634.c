void DocumentLoader::addSubresourceLoader(ResourceLoader* loader)
{
    if (!m_gotFirstByte)
        return;
    ASSERT(!m_subresourceLoaders.contains(loader));
    ASSERT(!mainResourceLoader() || mainResourceLoader() != loader);
    m_subresourceLoaders.add(loader);
}
