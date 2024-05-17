static void coroutine_fn pdu_complete(V9fsPDU *pdu, ssize_t len)
{
    int8_t id = pdu->id + 1;  
    V9fsState *s = pdu->s;
    int ret;

     
    bool discard = pdu->cancelled && len == -EINTR;
    if (discard) {
        trace_v9fs_rcancel(pdu->tag, pdu->id);
        pdu->size = 0;
        goto out_notify;
    }

    if (len < 0) {
        int err = -len;
        len = 7;

        if (s->proto_version != V9FS_PROTO_2000L) {
            V9fsString str;

            str.data = strerror(err);
            str.size = strlen(str.data);

            ret = pdu_marshal(pdu, len, "s", &str);
            if (ret < 0) {
                goto out_notify;
            }
            len += ret;
            id = P9_RERROR;
        }

        ret = pdu_marshal(pdu, len, "d", err);
        if (ret < 0) {
            goto out_notify;
        }
        len += ret;

        if (s->proto_version == V9FS_PROTO_2000L) {
            id = P9_RLERROR;
        }
        trace_v9fs_rerror(pdu->tag, pdu->id, err);  
    }

     
    if (pdu_marshal(pdu, 0, "dbw", (int32_t)len, id, pdu->tag) < 0) {
        goto out_notify;
    }

     
    pdu->size = len;
    pdu->id = id;

out_notify:
    pdu->s->transport->push_and_notify(pdu);

     
    if (!qemu_co_queue_next(&pdu->complete)) {
        pdu_free(pdu);
    }
}
