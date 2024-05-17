WebKitWebViewBase* webkitWebViewBaseCreate(WebContext* context, WebPageGroup* pageGroup)
{
    WebKitWebViewBase* webkitWebViewBase = WEBKIT_WEB_VIEW_BASE(g_object_new(WEBKIT_TYPE_WEB_VIEW_BASE, NULL));
    webkitWebViewBaseCreateWebPage(webkitWebViewBase, toAPI(context), toAPI(pageGroup));
    return webkitWebViewBase;
}
