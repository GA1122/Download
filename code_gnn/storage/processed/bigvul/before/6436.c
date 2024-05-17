MenuCacheDir* menu_cache_get_root_dir( MenuCache* cache )
{
    MenuCacheDir* dir = menu_cache_dup_root_dir(cache);
     
    if(dir)
        g_timeout_add_seconds(10, (GSourceFunc)menu_cache_item_unref, dir);
    return dir;
}
