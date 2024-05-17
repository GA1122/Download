void FrameLoaderClient::download(ResourceHandle* handle, const ResourceRequest& request, const ResourceRequest&, const ResourceResponse& response)
{
    WebKitNetworkRequest* networkRequest = webkit_network_request_new_with_core_request(request);
    WebKitWebView* view = getViewFromFrame(m_frame);

    webkit_web_view_request_download(view, networkRequest, response, handle);
    g_object_unref(networkRequest);
}
