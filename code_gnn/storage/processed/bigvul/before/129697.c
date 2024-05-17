void ResourceFetcher::didInitializeResourceLoader(ResourceLoader* loader)
{
    if (!m_document)
        return;
    if (!m_loaders)
        m_loaders = adoptPtr(new ResourceLoaderSet());
    ASSERT(!m_loaders->contains(loader));
    m_loaders->add(loader);
}
