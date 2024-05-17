double WebPageProxy::estimatedProgress() const
{
    if (!pendingAPIRequestURL().isNull())
        return initialProgressValue;
    return m_estimatedProgress; 
}
