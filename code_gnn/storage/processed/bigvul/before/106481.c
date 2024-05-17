void WebPageProxy::didFailLoadForResource(uint64_t frameID, uint64_t resourceIdentifier, const ResourceError& error)
{
    WebFrameProxy* frame = process()->webFrame(frameID);
    MESSAGE_CHECK(frame);

    m_resourceLoadClient.didFailLoadForResource(this, frame, resourceIdentifier, error);
}
