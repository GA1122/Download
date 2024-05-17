int ws_svr_stop(ws_svr *svr)
{
    int ret = nw_svr_stop(svr->raw_svr);
    if (ret < 0)
        return ret;

    return 0;
}
