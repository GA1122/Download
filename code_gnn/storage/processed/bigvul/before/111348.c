void WebPage::setExtraPluginDirectory(const BlackBerry::Platform::String& path)
{
    PluginDatabase* database = PluginDatabase::installedPlugins(true  );
    if (!database)
        return;
    
    Vector<String> pluginDirectories = database->pluginDirectories();
    if (path.empty() || pluginDirectories.contains(String(path)))
        return;

    pluginDirectories.append(path);
    database->setPluginDirectories(pluginDirectories);

    d->m_page->refreshPlugins(false  );

    if (d->m_webSettings->arePluginsEnabled())
        database->refresh();
}
