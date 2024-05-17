static int send_reply(nw_ses *ses, uint8_t opcode, void *payload, size_t payload_len)
{
    if (payload == NULL)
        payload_len = 0;

    static void *buf;
    static size_t buf_size = 1024;
    if (buf == NULL) {
        buf = malloc(1024);
        if (buf == NULL)
            return -1;
    }
    size_t require_len = 10 + payload_len;
    if (buf_size < require_len) {
        void *new = realloc(buf, require_len);
        if (new == NULL)
            return -1;
        buf = new;
        buf_size = require_len;
    }

    size_t pkg_len = 0;
    uint8_t *p = buf;
    p[0] = 0;
    p[0] |= 0x1 << 7;
    p[0] |= opcode;
    p[1] = 0;
    if (payload_len < 126) {
        uint8_t len = payload_len;
        p[1] |= len;
        pkg_len = 2;
    } else if (payload_len <= 0xffff) {
        p[1] |= 126;
        uint16_t len = htobe16((uint16_t)payload_len);
        memcpy(p + 2, &len, sizeof(len));
        pkg_len = 2 + sizeof(len);
    } else {
        p[1] |= 127;
        uint64_t len = htobe64(payload_len);
        memcpy(p + 2, &len, sizeof(len));
        pkg_len = 2 + sizeof(len);
    }

    if (payload) {
        memcpy(p + pkg_len, payload, payload_len);
        pkg_len += payload_len;
    }

    return nw_ses_send(ses, buf, pkg_len);
}
