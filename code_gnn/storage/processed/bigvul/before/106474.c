void WebPageProxy::didCommitLoadForFrame(uint64_t frameID, const String& mimeType, bool frameHasCustomRepresentation, const PlatformCertificateInfo& certificateInfo, CoreIPC::ArgumentDecoder* arguments)
{
    RefPtr<APIObject> userData;
    WebContextUserMessageDecoder messageDecoder(userData, context());
    if (!arguments->decode(messageDecoder))
        return;

#if PLATFORM(MAC) && !defined(BUILDING_ON_SNOW_LEOPARD)
    dismissCorrectionPanel(ReasonForDismissingCorrectionPanelIgnored);
#endif

    WebFrameProxy* frame = process()->webFrame(frameID);
    MESSAGE_CHECK(frame);

    clearLoadDependentCallbacks();

    frame->didCommitLoad(mimeType, certificateInfo);

    if (frame->isMainFrame()) {
        m_mainFrameHasCustomRepresentation = frameHasCustomRepresentation;
        m_pageClient->didCommitLoadForMainFrame(frameHasCustomRepresentation);
    }

    m_loaderClient.didCommitLoadForFrame(this, frame, userData.get());
}
