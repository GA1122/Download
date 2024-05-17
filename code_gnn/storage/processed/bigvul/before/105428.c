GtkTargetList* webkit_web_view_get_copy_target_list(WebKitWebView* webView)
{
    return pasteboardHelperInstance()->targetList();
}
