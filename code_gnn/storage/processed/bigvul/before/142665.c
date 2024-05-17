void FrameLoader::applyUserAgent(ResourceRequest& request)
{
    String userAgent = this->userAgent();
    ASSERT(!userAgent.isNull());
    request.setHTTPUserAgent(AtomicString(userAgent));
}
