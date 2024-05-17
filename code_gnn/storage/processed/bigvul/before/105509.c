void webkit_web_view_set_editable(WebKitWebView* webView, gboolean flag)
{
    g_return_if_fail(WEBKIT_IS_WEB_VIEW(webView));

    flag = flag != FALSE;
    if (flag == webkit_web_view_get_editable(webView))
        return;

    core(webView)->setEditable(flag);

    Frame* frame = core(webView)->mainFrame();
    g_return_if_fail(frame);

    if (flag) {
        frame->editor()->applyEditingStyleToBodyElement();
    }
    g_object_notify(G_OBJECT(webView), "editable");
}
