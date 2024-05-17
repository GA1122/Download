void MainResourceLoader::substituteMIMETypeFromPluginDatabase(const ResourceResponse& r)
{
    if (!m_frame->loader()->subframeLoader()->allowPlugins(NotAboutToInstantiatePlugin))
        return;

    String filename = r.url().lastPathComponent();
    if (filename.endsWith("/"))
        return;

    int extensionPos = filename.reverseFind('.');
    if (extensionPos == -1)
        return;

    String extension = filename.substring(extensionPos + 1);
    String mimeType = PluginDatabase::installedPlugins()->MIMETypeForExtension(extension);
    if (!mimeType.isEmpty()) {
        ResourceResponse* response = const_cast<ResourceResponse*>(&r);
        response->setMimeType(mimeType);
    }
}
