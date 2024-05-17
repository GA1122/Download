ResourceError FrameLoaderClient::cancelledError(const ResourceRequest& request)
{
    return ResourceError(g_quark_to_string(WEBKIT_NETWORK_ERROR), WEBKIT_NETWORK_ERROR_CANCELLED,
                         request.url().string(), _("Load request cancelled"));
}
