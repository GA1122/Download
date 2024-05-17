ResourceError FrameLoaderClient::interruptForPolicyChangeError(const ResourceRequest& request)
{
    return ResourceError(g_quark_to_string(WEBKIT_POLICY_ERROR), WEBKIT_POLICY_ERROR_FRAME_LOAD_INTERRUPTED_BY_POLICY_CHANGE,
                         request.url().string(), _("Frame load was interrupted"));
}
