ResourceError FrameLoaderClient::blockedError(const ResourceRequest& request)
{
    return ResourceError(g_quark_to_string(WEBKIT_POLICY_ERROR), WEBKIT_POLICY_ERROR_CANNOT_USE_RESTRICTED_PORT,
                         request.url().string(), _("Not allowed to use restricted network port"));
}
