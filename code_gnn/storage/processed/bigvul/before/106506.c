void WebPageProxy::didSameDocumentNavigationForFrame(uint64_t frameID, uint32_t opaqueSameDocumentNavigationType, const String& url, CoreIPC::ArgumentDecoder* arguments)
{
    RefPtr<APIObject> userData;
    WebContextUserMessageDecoder messageDecoder(userData, context());
    if (!arguments->decode(messageDecoder))
        return;

    WebFrameProxy* frame = process()->webFrame(frameID);
    MESSAGE_CHECK(frame);

    frame->didSameDocumentNavigation(url);

    m_loaderClient.didSameDocumentNavigationForFrame(this, frame, static_cast<SameDocumentNavigationType>(opaqueSameDocumentNavigationType), userData.get());
}
