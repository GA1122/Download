static void didRunInsecureContentForFrame(WKPageRef page, WKFrameRef frame, WKTypeRef userData, const void *clientInfo)
{
    webkitWebViewInsecureContentDetected(WEBKIT_WEB_VIEW(clientInfo), WEBKIT_INSECURE_CONTENT_RUN);
}