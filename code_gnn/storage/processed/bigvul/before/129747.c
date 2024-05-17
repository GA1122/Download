void ResourceFetcher::stopFetching()
{
    if (m_multipartLoaders)
        m_multipartLoaders->cancelAll();
    if (m_loaders)
        m_loaders->cancelAll();
}
