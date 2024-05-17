void WebPageProxy::didSendRequestForResource(uint64_t frameID, uint64_t resourceIdentifier, const ResourceRequest& request, const ResourceResponse& redirectResponse)
{
    WebFrameProxy* frame = process()->webFrame(frameID);
    MESSAGE_CHECK(frame);

    m_resourceLoadClient.didSendRequestForResource(this, frame, resourceIdentifier, request, redirectResponse);
}
