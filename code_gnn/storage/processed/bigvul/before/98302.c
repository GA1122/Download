void FrameLoaderClient::dispatchDidFailLoading(WebCore::DocumentLoader* loader, unsigned long identifier, const ResourceError& error)
{
    static_cast<WebKit::DocumentLoader*>(loader)->decreaseLoadCount(identifier);

    notImplemented();
}
