void MainResourceLoader::handleEmptyLoad(const KURL& url, bool forURLScheme)
{
    String mimeType;
    if (forURLScheme)
        mimeType = frameLoader()->generatedMIMETypeForURLScheme(url.protocol());
    else
        mimeType = "text/html";
    
    ResourceResponse response(url, mimeType, 0, String(), String());
    didReceiveResponse(response);
}
