void WebPageProxy::didInitiateLoadForResource(uint64_t frameID, uint64_t resourceIdentifier, const ResourceRequest& request, bool pageIsProvisionallyLoading)
{
    WebFrameProxy* frame = process()->webFrame(frameID);
    MESSAGE_CHECK(frame);

    m_resourceLoadClient.didInitiateLoadForResource(this, frame, resourceIdentifier, request, pageIsProvisionallyLoading);
}
