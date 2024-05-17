static void on_privdata_free(void *svr, void *privdata)
{
    struct clt_info *info = privdata;
    if (info->field) {
        sdsfree(info->field);
    }
    if (info->value) {
        sdsfree(info->value);
    }
    if (info->remote) {
        sdsfree(info->remote);
    }
    if (info->url) {
        sdsfree(info->url);
    }
    if (info->message) {
        sdsfree(info->message);
    }
    if (info->request) {
        http_request_release(info->request);
    }
    ws_svr *w_svr = ((nw_svr *)svr)->privdata;
    nw_cache_free(w_svr->privdata_cache, privdata);
}
