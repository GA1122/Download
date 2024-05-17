ResourceError FrameLoaderClient::cannotShowURLError(const ResourceRequest& request)
{
    return ResourceError(g_quark_to_string(WEBKIT_POLICY_ERROR), WEBKIT_POLICY_ERROR_CANNOT_SHOW_URL,
                         request.url().string(), _("URL cannot be shown"));
}
