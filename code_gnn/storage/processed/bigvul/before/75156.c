static void complete_nread_ascii(conn *c) {
    assert(c != NULL);

    item *it = c->item;
    int comm = c->cmd;
    enum store_item_type ret;
    bool is_valid = false;

    pthread_mutex_lock(&c->thread->stats.mutex);
    c->thread->stats.slab_stats[ITEM_clsid(it)].set_cmds++;
    pthread_mutex_unlock(&c->thread->stats.mutex);

    if ((it->it_flags & ITEM_CHUNKED) == 0) {
        if (strncmp(ITEM_data(it) + it->nbytes - 2, "\r\n", 2) == 0) {
            is_valid = true;
        }
    } else {
        char buf[2];
         
        item_chunk *ch = (item_chunk *) c->ritem;
        assert(ch->used != 0);
         
        if (ch->used > 1) {
            buf[0] = ch->data[ch->used - 2];
            buf[1] = ch->data[ch->used - 1];
        } else {
            assert(ch->prev);
            assert(ch->used == 1);
            buf[0] = ch->prev->data[ch->prev->used - 1];
            buf[1] = ch->data[ch->used - 1];
        }
        if (strncmp(buf, "\r\n", 2) == 0) {
            is_valid = true;
        } else {
            assert(1 == 0);
        }
    }

    if (!is_valid) {
        out_string(c, "CLIENT_ERROR bad data chunk");
    } else {
      ret = store_item(it, comm, c);

#ifdef ENABLE_DTRACE
      uint64_t cas = ITEM_get_cas(it);
      switch (c->cmd) {
      case NREAD_ADD:
          MEMCACHED_COMMAND_ADD(c->sfd, ITEM_key(it), it->nkey,
                                (ret == 1) ? it->nbytes : -1, cas);
          break;
      case NREAD_REPLACE:
          MEMCACHED_COMMAND_REPLACE(c->sfd, ITEM_key(it), it->nkey,
                                    (ret == 1) ? it->nbytes : -1, cas);
          break;
      case NREAD_APPEND:
          MEMCACHED_COMMAND_APPEND(c->sfd, ITEM_key(it), it->nkey,
                                   (ret == 1) ? it->nbytes : -1, cas);
          break;
      case NREAD_PREPEND:
          MEMCACHED_COMMAND_PREPEND(c->sfd, ITEM_key(it), it->nkey,
                                    (ret == 1) ? it->nbytes : -1, cas);
          break;
      case NREAD_SET:
          MEMCACHED_COMMAND_SET(c->sfd, ITEM_key(it), it->nkey,
                                (ret == 1) ? it->nbytes : -1, cas);
          break;
      case NREAD_CAS:
          MEMCACHED_COMMAND_CAS(c->sfd, ITEM_key(it), it->nkey, it->nbytes,
                                cas);
          break;
      }
#endif

      switch (ret) {
      case STORED:
          out_string(c, "STORED");
          break;
      case EXISTS:
          out_string(c, "EXISTS");
          break;
      case NOT_FOUND:
          out_string(c, "NOT_FOUND");
          break;
      case NOT_STORED:
          out_string(c, "NOT_STORED");
          break;
      default:
          out_string(c, "SERVER_ERROR Unhandled storage type.");
      }

    }

    item_remove(c->item);        
    c->item = 0;
}