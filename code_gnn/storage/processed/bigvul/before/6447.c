MenuCacheDir* menu_cache_item_get_parent( MenuCacheItem* item )
{
    MenuCacheDir* dir = menu_cache_item_dup_parent(item);
     
    if(dir)
        g_timeout_add_seconds(10, (GSourceFunc)menu_cache_item_unref, dir);
    return dir;
}