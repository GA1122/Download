ResourceError FrameLoaderClient::fileDoesNotExistError(const ResourceResponse& response)
{
    return ResourceError(g_quark_to_string(WEBKIT_NETWORK_ERROR), WEBKIT_NETWORK_ERROR_FILE_DOES_NOT_EXIST,
                         response.url().string(), _("File does not exist"));
}
