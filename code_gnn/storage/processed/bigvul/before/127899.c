StoredCredentials DocumentThreadableLoader::effectiveAllowCredentials() const
{
    if (m_forceDoNotAllowStoredCredentials)
        return DoNotAllowStoredCredentials;
    return m_resourceLoaderOptions.allowCredentials;
}
