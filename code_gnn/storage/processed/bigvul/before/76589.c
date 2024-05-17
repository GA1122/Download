int ws_send_binary(nw_ses *ses, void *data, size_t size)
{
    return send_reply(ses, 0x2, data, size);
}
