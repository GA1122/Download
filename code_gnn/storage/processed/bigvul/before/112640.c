void DocumentLoader::cancelMainResourceLoad(const ResourceError& resourceError)
{
    RefPtr<DocumentLoader> protect(this);
    ResourceError error = resourceError.isNull() ? frameLoader()->cancelledError(m_request) : resourceError;

    m_dataLoadTimer.stop();
    if (m_waitingForContentPolicy) {
        frameLoader()->policyChecker()->cancelCheck();
        ASSERT(m_waitingForContentPolicy);
        m_waitingForContentPolicy = false;
    }

    if (mainResourceLoader())
        mainResourceLoader()->cancel(error);

    mainReceivedError(error);
}
