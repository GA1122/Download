bool WebPageProxy::canShowMIMEType(const String& mimeType) const
{
    if (MIMETypeRegistry::isSupportedNonImageMIMEType(mimeType))
        return true;

    if (MIMETypeRegistry::isSupportedImageMIMEType(mimeType))
        return true;

    if (mimeType.startsWith("text/", false))
        return !MIMETypeRegistry::isUnsupportedTextMIMEType(mimeType);

    String newMimeType = mimeType;
    PluginInfoStore::Plugin plugin = context()->pluginInfoStore()->findPlugin(newMimeType, KURL());
    if (!plugin.path.isNull())
        return true;

    return false;
}
