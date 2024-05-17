void menu_cache_unref(MenuCache* cache)
{
     
     
    MENU_CACHE_LOCK;
    if( g_atomic_int_dec_and_test(&cache->n_ref) )
    {
         
        unregister_menu_from_server( cache );
         
        g_hash_table_remove( hash, cache->menu_name );
        if( g_hash_table_size(hash) == 0 )
        {
             
            g_hash_table_destroy(hash);

             
            G_LOCK(connect);
            shutdown(server_fd, SHUT_RDWR);  
            server_fd = -1;
            G_UNLOCK(connect);
            hash = NULL;
        }
        MENU_CACHE_UNLOCK;

        if(G_LIKELY(cache->thr))
        {
            g_cancellable_cancel(cache->cancellable);
            g_thread_join(cache->thr);
        }
        g_object_unref(cache->cancellable);
        if( G_LIKELY(cache->root_dir) )
        {
             
            menu_cache_item_unref( MENU_CACHE_ITEM(cache->root_dir) );
             
        }
        g_free( cache->cache_file );
        g_free( cache->menu_name );
        g_free(cache->reg);
         
        g_strfreev(cache->known_des);
        g_slist_free(cache->notifiers);
        g_slice_free( MenuCache, cache );
    }
    else
        MENU_CACHE_UNLOCK;
}
