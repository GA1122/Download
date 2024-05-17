void WebPageProxy::didReceiveResponseForResource(uint64_t frameID, uint64_t resourceIdentifier, const ResourceResponse& response)
{
    WebFrameProxy* frame = process()->webFrame(frameID);
    MESSAGE_CHECK(frame);

    m_resourceLoadClient.didReceiveResponseForResource(this, frame, resourceIdentifier, response);
}
