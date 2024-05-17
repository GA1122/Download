int ws_svr_start(ws_svr *svr)
{
    int ret = nw_svr_start(svr->raw_svr);
    if (ret < 0)
        return ret;

    return 0;
}
