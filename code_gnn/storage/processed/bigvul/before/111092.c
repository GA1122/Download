void WebPage::clearPluginSiteData()
{
    PluginDatabase* database = PluginDatabase::installedPlugins(true);

    if (!database)
        return;

    Vector<PluginPackage*> plugins = database->plugins();

    Vector<PluginPackage*>::const_iterator end = plugins.end();
    for (Vector<PluginPackage*>::const_iterator it = plugins.begin(); it != end; ++it)
        (*it)->clearSiteData(String());
}
