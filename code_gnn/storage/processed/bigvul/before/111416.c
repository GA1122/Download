void WebPage::updateDisabledPluginFiles(const BlackBerry::Platform::String& fileName, bool disabled)
{
    PluginDatabase* database = PluginDatabase::installedPlugins(true  );
    if (!database)
        return;

    if (disabled) {
        if (!database->addDisabledPluginFile(fileName))
            return;
    } else {
        if (!database->removeDisabledPluginFile(fileName))
            return;
    }


    d->m_page->refreshPlugins(false  );

    if (d->m_webSettings->arePluginsEnabled())
        database->refresh();
}
