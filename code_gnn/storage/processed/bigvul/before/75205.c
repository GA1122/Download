static int read_into_chunked_item(conn *c) {
    int total = 0;
    int res;
    assert(c->rcurr != c->ritem);

    while (c->rlbytes > 0) {
        item_chunk *ch = (item_chunk *)c->ritem;
        assert(ch->used <= ch->size);
        if (ch->size == ch->used) {
            if (ch->next) {
                c->ritem = (char *) ch->next;
            } else {
                 
                c->ritem = (char *) do_item_alloc_chunk(ch, c->rlbytes +
                       ((c->protocol == binary_prot) ? 2 : 0));
                if (!c->ritem) {
                    total = -2;
                    break;
                }
                continue;
            }
        }

        int unused = ch->size - ch->used;
         
        if (c->rbytes > 0) {
            total = 0;
            int tocopy = c->rbytes > c->rlbytes ? c->rlbytes : c->rbytes;
            tocopy = tocopy > unused ? unused : tocopy;
            if (c->ritem != c->rcurr) {
                memmove(ch->data + ch->used, c->rcurr, tocopy);
            }
            total += tocopy;
            c->rlbytes -= tocopy;
            c->rcurr += tocopy;
            c->rbytes -= tocopy;
            ch->used += tocopy;
            if (c->rlbytes == 0) {
                break;
            }
        } else {
             
            res = read(c->sfd, ch->data + ch->used,
                    (unused > c->rlbytes ? c->rlbytes : unused));
            if (res > 0) {
                pthread_mutex_lock(&c->thread->stats.mutex);
                c->thread->stats.bytes_read += res;
                pthread_mutex_unlock(&c->thread->stats.mutex);
                ch->used += res;
                total += res;
                c->rlbytes -= res;
            } else {
                 
                total = res;
                break;
            }
        }
    }

     
    if (c->rlbytes == 0 && c->protocol == binary_prot && total >= 0) {
        item_chunk *ch = (item_chunk *)c->ritem;
        if (ch->size - ch->used < 2) {
            c->ritem = (char *) do_item_alloc_chunk(ch, 2);
            if (!c->ritem) {
                total = -2;
            }
        }
    }
    return total;
}