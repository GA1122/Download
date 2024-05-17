ResourceError FrameLoaderClient::cannotShowMIMETypeError(const ResourceResponse& response)
{
    return ResourceError(g_quark_to_string(WEBKIT_POLICY_ERROR), WEBKIT_POLICY_ERROR_CANNOT_SHOW_MIME_TYPE,
                         response.url().string(), _("Content with the specified MIME type cannot be shown"));
}
