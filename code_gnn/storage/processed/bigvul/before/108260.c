bool FrameLoader::isMixedContent(SecurityOrigin* context, const KURL& url)
{
    if (context->protocol() != "https")
        return false;   

    if (!url.isValid() || SchemeRegistry::shouldTreatURLSchemeAsSecure(url.protocol()))
        return false;   

    return true;
}
