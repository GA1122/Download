char* menu_cache_dir_make_path( MenuCacheDir* dir )
{
    GString* path = g_string_sized_new(1024);
    MenuCacheItem* it;

    MENU_CACHE_LOCK;
    while( (it = MENU_CACHE_ITEM(dir)) )  
    {
        g_string_prepend( path, menu_cache_item_get_id(it) );
        g_string_prepend_c( path, '/' );
         
        dir = it->parent;
    }
    MENU_CACHE_UNLOCK;
    return g_string_free( path, FALSE );
}
