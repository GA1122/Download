void WebPageProxy::didReceiveServerRedirectForProvisionalLoadForFrame(uint64_t frameID, const String& url, CoreIPC::ArgumentDecoder* arguments)
{
    RefPtr<APIObject> userData;
    WebContextUserMessageDecoder messageDecoder(userData, context());
    if (!arguments->decode(messageDecoder))
        return;

    WebFrameProxy* frame = process()->webFrame(frameID);
    MESSAGE_CHECK(frame);

    frame->didReceiveServerRedirectForProvisionalLoad(url);

    m_loaderClient.didReceiveServerRedirectForProvisionalLoadForFrame(this, frame, userData.get());
}
