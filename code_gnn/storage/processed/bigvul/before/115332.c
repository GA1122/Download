static void didChangeProgress(WKPageRef page, const void* clientInfo)
{
    webkitWebViewSetEstimatedLoadProgress(WEBKIT_WEB_VIEW(clientInfo), WKPageGetEstimatedProgress(page));
}
