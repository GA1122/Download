static void didReceiveTitleForFrame(WKPageRef page, WKStringRef titleRef, WKFrameRef frameRef, WKTypeRef, const void* clientInfo)
{
    if (!WKFrameIsMainFrame(frameRef))
        return;

    webkitWebViewSetTitle(WEBKIT_WEB_VIEW(clientInfo), toImpl(titleRef)->string().utf8());
}
