int rpc_send(nw_ses *ses, rpc_pkg *pkg)
{
    void *data;
    uint32_t size;
    int ret = rpc_pack(pkg, &data, &size);
    if (ret < 0)
        return ret;
    return nw_ses_send(ses, data, size);
}
