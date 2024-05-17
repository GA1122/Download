static gboolean reload_notify(gpointer data)
{
    MenuCache* cache = (MenuCache*)data;
    GSList* l;
    MENU_CACHE_LOCK;
     
    for( l = cache->notifiers; l; l = l->next )
    {
        CacheReloadNotifier* n = (CacheReloadNotifier*)l->data;
        if(n->func)
            n->func( cache, n->user_data );
    }
    MENU_CACHE_UNLOCK;
    return FALSE;
}
