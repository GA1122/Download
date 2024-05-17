GtkTargetList* webkit_web_view_get_paste_target_list(WebKitWebView* webView)
{
    return pasteboardHelperInstance()->targetList();
}
