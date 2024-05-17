void FrameLoaderClient::startDownload(const ResourceRequest& request)
{
    WebKitNetworkRequest* networkRequest = webkit_network_request_new_with_core_request(request);
    WebKitWebView* view = getViewFromFrame(m_frame);

    webkit_web_view_request_download(view, networkRequest);
    g_object_unref(networkRequest);
}
