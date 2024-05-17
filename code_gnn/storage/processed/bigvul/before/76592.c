int ws_svr_broadcast_binary(ws_svr *svr, void *data, size_t size)
{
    return broadcast_message(svr, 0x2, data, size);
}
