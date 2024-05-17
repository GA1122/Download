String WebInspectorProxy::inspectorPageURL() const
{
    GOwnPtr<gchar> filePath(g_build_filename(inspectorFilesBasePath(), "inspector.html", NULL));
    GOwnPtr<gchar> fileURI(g_filename_to_uri(filePath.get(), 0, 0));
    return WebCore::filenameToString(fileURI.get());
}
