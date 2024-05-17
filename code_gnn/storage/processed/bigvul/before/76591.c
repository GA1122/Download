void *ws_ses_privdata(nw_ses *ses)
{
    struct clt_info *info = ses->privdata;
    return info->privdata;
}
