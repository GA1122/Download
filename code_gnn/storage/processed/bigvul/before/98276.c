bool FrameLoaderClient::canShowMIMEType(const String& type) const
{
    return MIMETypeRegistry::isSupportedImageMIMEType(type) || MIMETypeRegistry::isSupportedNonImageMIMEType(type) ||
        PluginDatabase::installedPlugins()->isMIMETypeRegistered(type);
}
