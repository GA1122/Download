void xreadCommand(client *c) {
    long long timeout = -1;  
    long long count = 0;
    int streams_count = 0;
    int streams_arg = 0;
    int noack = 0;           
    #define STREAMID_STATIC_VECTOR_LEN 8
    streamID static_ids[STREAMID_STATIC_VECTOR_LEN];
    streamID *ids = static_ids;
    streamCG **groups = NULL;
    int xreadgroup = sdslen(c->argv[0]->ptr) == 10;  
    robj *groupname = NULL;
    robj *consumername = NULL;

     
    for (int i = 1; i < c->argc; i++) {
        int moreargs = c->argc-i-1;
        char *o = c->argv[i]->ptr;
        if (!strcasecmp(o,"BLOCK") && moreargs) {
            i++;
            if (getTimeoutFromObjectOrReply(c,c->argv[i],&timeout,
                UNIT_MILLISECONDS) != C_OK) return;
        } else if (!strcasecmp(o,"COUNT") && moreargs) {
            i++;
            if (getLongLongFromObjectOrReply(c,c->argv[i],&count,NULL) != C_OK)
                return;
            if (count < 0) count = 0;
        } else if (!strcasecmp(o,"STREAMS") && moreargs) {
            streams_arg = i+1;
            streams_count = (c->argc-streams_arg);
            if ((streams_count % 2) != 0) {
                addReplyError(c,"Unbalanced XREAD list of streams: "
                                "for each stream key an ID or '$' must be "
                                "specified.");
                return;
            }
            streams_count /= 2;  
            break;
        } else if (!strcasecmp(o,"GROUP") && moreargs >= 2) {
            if (!xreadgroup) {
                addReplyError(c,"The GROUP option is only supported by "
                                "XREADGROUP. You called XREAD instead.");
                return;
            }
            groupname = c->argv[i+1];
            consumername = c->argv[i+2];
            i += 2;
        } else if (!strcasecmp(o,"NOACK")) {
            if (!xreadgroup) {
                addReplyError(c,"The NOACK option is only supported by "
                                "XREADGROUP. You called XREAD instead.");
                return;
            }
            noack = 1;
        } else {
            addReply(c,shared.syntaxerr);
            return;
        }
    }

     
    if (streams_arg == 0) {
        addReply(c,shared.syntaxerr);
        return;
    }

     
    if (xreadgroup && groupname == NULL) {
        addReplyError(c,"Missing GROUP option for XREADGROUP");
        return;
    }

     
    if (streams_count > STREAMID_STATIC_VECTOR_LEN)
        ids = zmalloc(sizeof(streamID)*streams_count);
    if (groupname) groups = zmalloc(sizeof(streamCG*)*streams_count);

    for (int i = streams_arg + streams_count; i < c->argc; i++) {
         
        int id_idx = i - streams_arg - streams_count;
        robj *key = c->argv[i-streams_count];
        robj *o;
        streamCG *group = NULL;

         
        if (groupname) {
            o = lookupKeyRead(c->db,key);
            if (o && checkType(c,o,OBJ_STREAM)) goto cleanup;
            if (o == NULL ||
                (group = streamLookupCG(o->ptr,groupname->ptr)) == NULL)
            {
                addReplyErrorFormat(c, "-NOGROUP No such key '%s' or consumer "
                                       "group '%s' in XREADGROUP with GROUP "
                                       "option",
                                    (char*)key->ptr,(char*)groupname->ptr);
                goto cleanup;
            }
            groups[id_idx] = group;
        }

        if (strcmp(c->argv[i]->ptr,"$") == 0) {
            o = lookupKeyRead(c->db,key);
            if (o && checkType(c,o,OBJ_STREAM)) goto cleanup;
            if (o) {
                stream *s = o->ptr;
                ids[id_idx] = s->last_id;
            } else {
                ids[id_idx].ms = 0;
                ids[id_idx].seq = 0;
            }
            continue;
        } else if (strcmp(c->argv[i]->ptr,">") == 0) {
            if (!xreadgroup || groupname == NULL) {
                addReplyError(c,"The > ID can be specified only when calling "
                                "XREADGROUP using the GROUP <group> "
                                "<consumer> option.");
                goto cleanup;
            }
            ids[id_idx] = group->last_id;
            continue;
        }
        if (streamParseIDOrReply(c,c->argv[i],ids+id_idx,0) != C_OK)
            goto cleanup;
    }

     
    size_t arraylen = 0;
    void *arraylen_ptr = NULL;
    for (int i = 0; i < streams_count; i++) {
        robj *o = lookupKeyRead(c->db,c->argv[streams_arg+i]);
        if (o == NULL) continue;
        stream *s = o->ptr;
        streamID *gt = ids+i;  
        if (s->last_id.ms > gt->ms ||
            (s->last_id.ms == gt->ms && s->last_id.seq > gt->seq))
        {
            arraylen++;
            if (arraylen == 1) arraylen_ptr = addDeferredMultiBulkLength(c);
             
            streamID start = *gt;
            start.seq++;  

             
            addReplyMultiBulkLen(c,2);
            addReplyBulk(c,c->argv[i+streams_arg]);
            streamConsumer *consumer = NULL;
            if (groups) consumer = streamLookupConsumer(groups[i],
                                                        consumername->ptr,1);
            streamPropInfo spi = {c->argv[i+streams_arg],groupname};
            streamReplyWithRange(c,s,&start,NULL,count,0,
                                 groups ? groups[i] : NULL,
                                 consumer, noack, &spi);
            if (groups) server.dirty++;
        }
    }

      
    if (arraylen) {
        setDeferredMultiBulkLength(c,arraylen_ptr,arraylen);
        goto cleanup;
    }

     
    if (timeout != -1) {
         
        if (c->flags & CLIENT_MULTI) {
            addReply(c,shared.nullmultibulk);
            goto cleanup;
        }
        blockForKeys(c, BLOCKED_STREAM, c->argv+streams_arg, streams_count,
                     timeout, NULL, ids);
         
        c->bpop.xread_count = count ? count : XREAD_BLOCKED_DEFAULT_COUNT;

         
        if (groupname) {
            incrRefCount(groupname);
            incrRefCount(consumername);
            c->bpop.xread_group = groupname;
            c->bpop.xread_consumer = consumername;
        } else {
            c->bpop.xread_group = NULL;
            c->bpop.xread_consumer = NULL;
        }
        goto cleanup;
    }

     
    addReply(c,shared.nullmultibulk);
     

cleanup:  

     
    preventCommandPropagation(c);
    if (ids != static_ids) zfree(ids);
    zfree(groups);
}
