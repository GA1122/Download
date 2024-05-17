void DocumentThreadableLoader::handleSuccessfulFinish(unsigned long identifier, double finishTime)
{
    ASSERT(m_fallbackRequestForServiceWorker.isNull());

    if (!m_actualRequest.isNull()) {
        m_timeoutTimer.stop();
        ASSERT(!m_sameOriginRequest);
        ASSERT(m_options.crossOriginRequestPolicy == UseAccessControl);
        loadActualRequest();
        return;
    }

    ThreadableLoaderClient* client = m_client;
    m_client = nullptr;
    if (m_async) {
        m_timeoutTimer.stop();
        m_requestStartedSeconds = 0.0;
    }
    client->didFinishLoading(identifier, finishTime);
}
