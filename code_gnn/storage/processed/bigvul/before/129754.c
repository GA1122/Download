void ResourceFetcher::willTerminateResourceLoader(ResourceLoader* loader)
{
    if (m_loaders && m_loaders->contains(loader))
        m_loaders->remove(loader);
    if (m_multipartLoaders && m_multipartLoaders->contains(loader))
        m_multipartLoaders->remove(loader);
    if (LocalFrame* frame = this->frame())
        frame->loader().checkLoadComplete();
}
