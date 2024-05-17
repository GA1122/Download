void WebPageProxy::didFinishLoadForResource(uint64_t frameID, uint64_t resourceIdentifier)
{
    WebFrameProxy* frame = process()->webFrame(frameID);
    MESSAGE_CHECK(frame);

    m_resourceLoadClient.didFinishLoadForResource(this, frame, resourceIdentifier);
}
