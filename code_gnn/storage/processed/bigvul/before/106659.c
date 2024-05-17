void WebPageProxy::unableToImplementPolicy(uint64_t frameID, const WebCore::ResourceError& error, CoreIPC::ArgumentDecoder* arguments)
{
    RefPtr<APIObject> userData;
    WebContextUserMessageDecoder messageDecoder(userData, context());
    if (!arguments->decode(messageDecoder))
        return;
    
    WebFrameProxy* frame = process()->webFrame(frameID);
    MESSAGE_CHECK(frame);

    m_policyClient.unableToImplementPolicy(this, frame, error, userData.get());
}
