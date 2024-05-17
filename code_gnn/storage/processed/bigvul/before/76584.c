static void on_recv_pkg(nw_ses *ses, void *data, size_t size)
{
    struct clt_info *info = ses->privdata;
    ws_svr *svr = ws_svr_from_ses(ses);
    info->last_activity = current_timestamp();
    if (!info->upgrade) {
        size_t nparsed = http_parser_execute(&info->parser, &svr->settings, data, size);
        if (!info->parser.upgrade && nparsed != size) {
            log_error("peer: %s http parse error: %s (%s)", nw_sock_human_addr(&ses->peer_addr),
                    http_errno_description(HTTP_PARSER_ERRNO(&info->parser)),
                    http_errno_name(HTTP_PARSER_ERRNO(&info->parser)));
            nw_svr_close_clt(svr->raw_svr, ses);
        }
        return;
    }

    switch (info->frame.opcode) {
    case 0x8:
        nw_svr_close_clt(svr->raw_svr, ses);
        return;
    case 0x9:
        send_pong_message(ses);
        return;
    case 0xa:
        return;
    }

    if (info->message == NULL)
        info->message = sdsempty();
    info->message = sdscatlen(info->message, info->frame.payload, info->frame.payload_len);
    if (info->frame.fin) {
        int ret = svr->type.on_message(ses, info->remote, info->url, info->message, sdslen(info->message));
        if (ses->id != 0) {
            if (ret < 0) {
                nw_svr_close_clt(svr->raw_svr, ses);
            } else {
                sdsfree(info->message);
                info->message = NULL;
            }
        }
    }
}
