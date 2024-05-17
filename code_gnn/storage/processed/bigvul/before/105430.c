webkit_web_view_get_dom_document(WebKitWebView* webView)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), 0);

    Frame* coreFrame = core(webView)->mainFrame();
    if (!coreFrame)
        return 0;

    Document* doc = coreFrame->document();
    if (!doc)
        return 0;

    return kit(doc);
}
