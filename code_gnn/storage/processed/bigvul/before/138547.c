void resetPluginCache(bool reloadPages)
{
    ASSERT(!reloadPages);
    Page::refreshPlugins();
}
