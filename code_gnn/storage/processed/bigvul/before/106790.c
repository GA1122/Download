bool WebPage::platformCanHandleRequest(const WebCore::ResourceRequest& request)
{
#if USE(CFNETWORK)
    return CFURLProtocolCanHandleRequest(request.cfURLRequest());
#else
    return true;
#endif
}
