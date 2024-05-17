void webkit_web_view_request_download(WebKitWebView* webView, WebKitNetworkRequest* request, const ResourceResponse& response, ResourceHandle* handle)
{
    g_return_if_fail(WEBKIT_IS_WEB_VIEW(webView));

    WebKitDownload* download;

    if (handle)
        download = webkit_download_new_with_handle(request, handle, response);
    else
        download = webkit_download_new(request);

    gboolean handled;
    g_signal_emit(webView, webkit_web_view_signals[DOWNLOAD_REQUESTED], 0, download, &handled);

    if (!handled) {
        webkit_download_cancel(download);
        g_object_unref(download);
        return;
    }

     
    if (webkit_download_get_destination_uri(download))
        webkit_download_start(download);
}
