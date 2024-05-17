void WebPageProxy::didFailProvisionalLoadForFrame(uint64_t frameID, const ResourceError& error, CoreIPC::ArgumentDecoder* arguments)
{
    RefPtr<APIObject> userData;
    WebContextUserMessageDecoder messageDecoder(userData, context());
    if (!arguments->decode(messageDecoder))
        return;

    WebFrameProxy* frame = process()->webFrame(frameID);
    MESSAGE_CHECK(frame);

    frame->didFailProvisionalLoad();

    m_loaderClient.didFailProvisionalLoadWithErrorForFrame(this, frame, error, userData.get());
}
