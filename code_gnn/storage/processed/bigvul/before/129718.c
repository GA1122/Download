bool ResourceFetcher::isFetching() const
{
    return m_loaders && !m_loaders->isEmpty();
}
