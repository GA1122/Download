String WebInspectorProxy::inspectorBaseURL() const
{
    GOwnPtr<gchar> fileURI(g_filename_to_uri(inspectorFilesBasePath(), 0, 0));
    return WebCore::filenameToString(fileURI.get());
}
