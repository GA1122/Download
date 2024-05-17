GSList* menu_cache_dir_get_children( MenuCacheDir* dir )
{
     
    g_timeout_add_seconds(10, (GSourceFunc)menu_cache_item_unref,
                          menu_cache_item_ref(MENU_CACHE_ITEM(dir)));
    return dir->children;
}
