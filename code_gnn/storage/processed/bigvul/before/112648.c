void DocumentLoader::continueAfterContentPolicy(PolicyAction policy)
{
    ASSERT(m_waitingForContentPolicy);
    m_waitingForContentPolicy = false;
    if (isStopping())
        return;

    KURL url = m_request.url();
    const String& mimeType = m_response.mimeType();
    
    switch (policy) {
    case PolicyUse: {
        bool isRemoteWebArchive = (equalIgnoringCase("application/x-webarchive", mimeType)
#if PLATFORM(GTK)
            || equalIgnoringCase("message/rfc822", mimeType)
#endif
            || equalIgnoringCase("multipart/related", mimeType))
            && !m_substituteData.isValid() && !SchemeRegistry::shouldTreatURLSchemeAsLocal(url.protocol());
        if (!frameLoader()->client()->canShowMIMEType(mimeType) || isRemoteWebArchive) {
            frameLoader()->policyChecker()->cannotShowMIMEType(m_response);
            stopLoadingForPolicyChange();
            return;
        }
        break;
    }

    case PolicyDownload: {
        if (!m_mainResource) {
            mainReceivedError(frameLoader()->client()->cannotShowURLError(m_request));
            return;
        }
        InspectorInstrumentation::continueWithPolicyDownload(m_frame, this, mainResourceLoader()->identifier(), m_response);

        frameLoader()->setOriginalURLForDownloadRequest(m_request);
        frameLoader()->client()->convertMainResourceLoadToDownload(this, m_request, m_response);

        if (mainResourceLoader())
            mainResourceLoader()->didFail(interruptedForPolicyChangeError());
        return;
    }
    case PolicyIgnore:
        InspectorInstrumentation::continueWithPolicyIgnore(m_frame, this, mainResourceLoader()->identifier(), m_response);
        stopLoadingForPolicyChange();
        return;
    
    default:
        ASSERT_NOT_REACHED();
    }

    if (m_response.isHTTP()) {
        int status = m_response.httpStatusCode();
        if (status < 200 || status >= 300) {
            bool hostedByObject = frameLoader()->isHostedByObjectElement();

            frameLoader()->handleFallbackContent();

            if (hostedByObject)
                cancelMainResourceLoad(frameLoader()->cancelledError(m_request));
        }
    }

    if (!isStopping() && m_substituteData.isValid()) {
        if (m_substituteData.content()->size())
            dataReceived(0, m_substituteData.content()->data(), m_substituteData.content()->size());
        if (isLoadingMainResource())
            finishedLoading(0);
    }
}
