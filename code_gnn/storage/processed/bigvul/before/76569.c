static int broadcast_message(ws_svr *svr, uint8_t opcode, void *data, size_t size)
{
    nw_ses *curr = svr->raw_svr->clt_list_head;
    while (curr) {
        nw_ses *next = curr->next;
        struct clt_info *info = curr->privdata;
        if (info->upgrade) {
            int ret = send_reply(curr, opcode, data, size);
            if (ret < 0)
                return ret;
        }
        curr = next;
    }

    return 0;
}
