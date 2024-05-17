gboolean webkit_web_view_go_to_back_forward_item(WebKitWebView* webView, WebKitWebHistoryItem* item)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), FALSE);
    g_return_val_if_fail(WEBKIT_IS_WEB_HISTORY_ITEM(item), FALSE);

    WebKitWebBackForwardList* backForwardList = webkit_web_view_get_back_forward_list(webView);
    if (!webkit_web_back_forward_list_contains_item(backForwardList, item))
        return FALSE;

    core(webView)->goToItem(core(item), FrameLoadTypeIndexedBackForward);
    return TRUE;
}
