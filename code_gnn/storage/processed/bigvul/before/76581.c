static void on_new_connection(nw_ses *ses)
{
    log_trace("new connection from: %s", nw_sock_human_addr(&ses->peer_addr));
    struct clt_info *info = ses->privdata;
    memset(info, 0, sizeof(struct clt_info));
    info->ses = ses;
    info->last_activity = current_timestamp();
    http_parser_init(&info->parser, HTTP_REQUEST);
    info->parser.data = info;
}
