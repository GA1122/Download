static int decode_pkg(nw_ses *ses, void *data, size_t max)
{
    struct clt_info *info = ses->privdata;
    if (!info->upgrade) {
        return max;
    }

    if (max < 2)
        return 0;

    uint8_t *p = data;
    size_t pkg_size = 0;
    memset(&info->frame, 0, sizeof(info->frame));
    info->frame.fin = p[0] & 0x80;
    info->frame.opcode = p[0] & 0x0f;
    if (!is_good_opcode(info->frame.opcode))
        return -1;
    uint8_t mask = p[1] & 0x80;
    if (mask == 0)
        return -1;

    uint8_t len = p[1] & 0x7f;
    if (len < 126) {
        pkg_size = 2;
        info->frame.payload_len = len;
    } else if (len == 126) {
        pkg_size = 2 + 2;
        if (max < pkg_size)
            return 0;
        info->frame.payload_len = be16toh(*(uint16_t *)(p + 2));
    } else if (len == 127) {
        pkg_size = 2 + 8;
        if (max < pkg_size)
            return 0;
        info->frame.payload_len = be64toh(*(uint64_t *)(p + 2));
    }

    uint8_t masks[4];
    memcpy(masks, p + pkg_size, sizeof(masks));
    pkg_size += sizeof(masks);
    info->frame.payload = p + pkg_size;
    pkg_size += info->frame.payload_len;
    if (max < pkg_size)
        return 0;

    p = info->frame.payload;
    for (size_t i = 0; i < info->frame.payload_len; ++i) {
        p[i] = p[i] ^ masks[i & 3];
    }

    return pkg_size;
}
