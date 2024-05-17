bool FrameLoader::isMixedContent(SecurityOrigin* context, const KURL& url)
{
    if (context->protocol() != "https")
        return false;   

    if (url.protocolIs("https") || url.protocolIs("about") || url.protocolIs("data"))
        return false;   

    return true;
}
