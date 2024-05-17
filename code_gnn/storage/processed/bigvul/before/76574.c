static void on_connection_close(nw_ses *ses)
{
    log_trace("connection %s close", nw_sock_human_addr(&ses->peer_addr));
    struct clt_info *info = ses->privdata;
    struct ws_svr *svr = ws_svr_from_ses(ses);
    if (info->upgrade) {
        if (svr->type.on_close) {
            svr->type.on_close(ses, info->remote);
        }
        if (svr->type.on_privdata_free) {
            svr->type.on_privdata_free(svr, info->privdata);
        }
    }
}
