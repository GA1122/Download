ws_svr *ws_svr_from_ses(nw_ses *ses)
{
    return ((nw_svr *)ses->svr)->privdata;
}
