void DocumentLoader::finishedLoading(double finishTime)
{
#if !USE(CF)
    ASSERT(!m_frame->page()->defersLoading() || InspectorInstrumentation::isDebuggerPaused(m_frame));
#endif

    RefPtr<DocumentLoader> protect(this);

    if (m_identifierForLoadWithoutResourceLoader) {
        frameLoader()->notifier()->dispatchDidFinishLoading(this, m_identifierForLoadWithoutResourceLoader, finishTime);
        m_identifierForLoadWithoutResourceLoader = 0;
    }

#if USE(CONTENT_FILTERING)
    if (m_contentFilter && m_contentFilter->needsMoreData()) {
        m_contentFilter->finishedAddingData();
        int length;
        const char* data = m_contentFilter->getReplacementData(length);
        if (data)
            dataReceived(m_mainResource.get(), data, length);
    }
#endif

    maybeFinishLoadingMultipartContent();

    double responseEndTime = finishTime;
    if (!responseEndTime)
        responseEndTime = m_timeOfLastDataReceived;
    if (!responseEndTime)
        responseEndTime = monotonicallyIncreasingTime();
    timing()->setResponseEnd(responseEndTime);

    commitIfReady();
    if (!frameLoader())
        return;

    if (!maybeCreateArchive()) {
        if (!m_gotFirstByte)
            commitData(0, 0);
        frameLoader()->client()->finishedLoading(this);
    }

    m_writer.end();
    if (!m_mainDocumentError.isNull())
        return;
    clearMainResourceLoader();
    if (!frameLoader()->stateMachine()->creatingInitialEmptyDocument())
        frameLoader()->checkLoadComplete();

    if (m_frame) {
        if (m_mainResource && m_frame->document()->hasManifest())
            memoryCache()->remove(m_mainResource.get());
    }
    m_applicationCacheHost->finishedLoadingMainResource();
}
