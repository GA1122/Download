static void *on_privdata_alloc(void *svr)
{
    ws_svr *w_svr = ((nw_svr *)svr)->privdata;
    return nw_cache_alloc(w_svr->privdata_cache);
}
