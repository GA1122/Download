static void _get_extstore_cb(void *e, obj_io *io, int ret) {
    io_wrap *wrap = (io_wrap *)io->data;
    conn *c = wrap->c;
    assert(wrap->active == true);
    item *read_it = (item *)io->buf;
    bool miss = false;

    if (ret < 1) {
        miss = true;
    } else {
        uint32_t crc2;
        uint32_t crc = (uint32_t) read_it->exptime;
        int x;
        if (io->iov != NULL) {
            crc2 = crc32c(0, (char *)io->iov[0].iov_base+32, io->iov[0].iov_len-32);
            io->iov[0].iov_len = 0;
            for (x = 1; x < io->iovcnt; x++) {
                crc2 = crc32c(crc2, (char *)io->iov[x].iov_base, io->iov[x].iov_len);
            }
        } else {
            crc2 = crc32c(0, (char *)read_it+32, io->len-32);
        }

        if (crc != crc2) {
            miss = true;
            wrap->badcrc = true;
        }
    }

    if (miss) {
        int i;
        struct iovec *v;
        if (c->protocol == binary_prot) {
            protocol_binary_response_header *header =
                (protocol_binary_response_header *)c->wbuf;
            if (header->response.keylen) {
                write_bin_miss_response(c, ITEM_key(wrap->hdr_it), wrap->hdr_it->nkey);
            } else {
                write_bin_miss_response(c, 0, 0);
            }
        } else {
            for (i = 0; i < wrap->iovec_count; i++) {
                v = &c->iov[wrap->iovec_start + i];
                v->iov_len = 0;
                v->iov_base = NULL;
            }
        }
        wrap->miss = true;
    } else {
        assert(read_it->slabs_clsid != 0);
        if (io->iov == NULL) {
            c->iov[wrap->iovec_data].iov_base = ITEM_data(read_it);
            if (c->protocol == binary_prot)
                c->iov[wrap->iovec_data].iov_len -= 2;
        } else {
            if (c->protocol == binary_prot) {
                if (io->iov[io->iovcnt-1].iov_len >= 2) {
                    io->iov[io->iovcnt-1].iov_len -= 2;
                } else {
                    io->iov[io->iovcnt-1].iov_len = 0;
                    io->iov[io->iovcnt-2].iov_len -= 1;
                }
            }
        }
    }
    c->io_wrapleft--;
    wrap->active = false;

    if (c->io_wrapleft == 0) {
        assert(c->io_queued == true);
        c->io_queued = false;
        redispatch_conn(c);
    }
}
