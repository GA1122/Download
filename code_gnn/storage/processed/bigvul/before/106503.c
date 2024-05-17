void WebPageProxy::didRemoveFrameFromHierarchy(uint64_t frameID, CoreIPC::ArgumentDecoder* arguments)
{
    RefPtr<APIObject> userData;
    WebContextUserMessageDecoder messageDecoder(userData, context());
    if (!arguments->decode(messageDecoder))
        return;

    WebFrameProxy* frame = process()->webFrame(frameID);
    MESSAGE_CHECK(frame);

    frame->didRemoveFromHierarchy();

    m_loaderClient.didRemoveFrameFromHierarchy(this, frame, userData.get());
}
