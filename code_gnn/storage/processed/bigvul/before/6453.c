gboolean menu_cache_reload( MenuCache* cache )
{
    char* line;
    gsize len;
    GFile* file;
    GFileInputStream* istr = NULL;
    GDataInputStream* f;
    MenuCacheFileDir** all_used_files;
    int i, n;
    int ver_maj, ver_min;

    MENU_CACHE_LOCK;
    if (cache->reload_id)
        g_source_remove(cache->reload_id);
    cache->reload_id = 0;
    MENU_CACHE_UNLOCK;
    file = g_file_new_for_path(cache->cache_file);
    if(!file)
        return FALSE;
    istr = g_file_read(file, cache->cancellable, NULL);
    g_object_unref(file);
    if(!istr)
        return FALSE;
    f = g_data_input_stream_new(G_INPUT_STREAM(istr));
    g_object_unref(istr);
    if( ! f )
        return FALSE;

     
    line = g_data_input_stream_read_line(f, &len, cache->cancellable, NULL);
    if(G_LIKELY(line))
    {
        len = sscanf(line, "%d.%d", &ver_maj, &ver_min);
        g_free(line);
        if(len < 2)
            goto _fail;
        if( ver_maj != VER_MAJOR ||
            ver_min > VER_MINOR || ver_min < VER_MINOR_SUPPORTED )
            goto _fail;
    }
    else
        goto _fail;

    g_debug("menu cache: got file version 1.%d", ver_min);
     
    line = g_data_input_stream_read_line(f, &len, cache->cancellable, NULL);
    if(G_UNLIKELY(line == NULL))
        goto _fail;
    g_free(line);

     
    MENU_CACHE_LOCK;
    if(cache->notifiers == NULL)
    {
         
        MENU_CACHE_UNLOCK;
        goto _fail;
    }

     
    n = read_all_used_files( f, cache, &all_used_files );
    if (n <= 0)
    {
        MENU_CACHE_UNLOCK;
        goto _fail;
    }

     
    g_strfreev( cache->known_des );
    if( ! read_all_known_des( f, cache ) )
    {
        cache->known_des = NULL;
        MENU_CACHE_UNLOCK;
        for (i = 0; i < n; i++)
            menu_cache_file_dir_unref(all_used_files[i]);
        g_free(all_used_files);
_fail:
        g_object_unref(f);
        return FALSE;
    }
    cache->version = ver_min;

    if(cache->root_dir)
        menu_cache_item_unref( MENU_CACHE_ITEM(cache->root_dir) );

    cache->root_dir = (MenuCacheDir*)read_item( f, cache, all_used_files, n );
    g_object_unref(f);

    g_idle_add_full(G_PRIORITY_HIGH_IDLE, reload_notify, menu_cache_ref(cache),
                    (GDestroyNotify)menu_cache_unref);
    MENU_CACHE_UNLOCK;

    for (i = 0; i < n; i++)
        menu_cache_file_dir_unref(all_used_files[i]);
    g_free(all_used_files);

    return TRUE;
}
