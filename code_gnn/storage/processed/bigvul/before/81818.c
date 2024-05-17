void xpendingCommand(client *c) {
    int justinfo = c->argc == 3;  
    robj *key = c->argv[1];
    robj *groupname = c->argv[2];
    robj *consumername = (c->argc == 7) ? c->argv[6] : NULL;
    streamID startid, endid;
    long long count;

     
    if (c->argc != 3 && c->argc != 6 && c->argc != 7) {
        addReply(c,shared.syntaxerr);
        return;
    }

     
    if (c->argc >= 6) {
        if (getLongLongFromObjectOrReply(c,c->argv[5],&count,NULL) == C_ERR)
            return;
        if (streamParseIDOrReply(c,c->argv[3],&startid,0) == C_ERR)
            return;
        if (streamParseIDOrReply(c,c->argv[4],&endid,UINT64_MAX) == C_ERR)
            return;
    }

     
    robj *o = lookupKeyRead(c->db,c->argv[1]);
    streamCG *group;

    if (o && checkType(c,o,OBJ_STREAM)) return;
    if (o == NULL ||
        (group = streamLookupCG(o->ptr,groupname->ptr)) == NULL)
    {
        addReplyErrorFormat(c, "-NOGROUP No such key '%s' or consumer "
                               "group '%s'",
                               (char*)key->ptr,(char*)groupname->ptr);
        return;
    }

     
    if (justinfo) {
        addReplyMultiBulkLen(c,4);
         
        addReplyLongLong(c,raxSize(group->pel));
         
        if (raxSize(group->pel) == 0) {
            addReply(c,shared.nullbulk);  
            addReply(c,shared.nullbulk);  
            addReply(c,shared.nullmultibulk);  
        } else {
             
            raxIterator ri;
            raxStart(&ri,group->pel);
            raxSeek(&ri,"^",NULL,0);
            raxNext(&ri);
            streamDecodeID(ri.key,&startid);
            addReplyStreamID(c,&startid);

             
            raxSeek(&ri,"$",NULL,0);
            raxNext(&ri);
            streamDecodeID(ri.key,&endid);
            addReplyStreamID(c,&endid);
            raxStop(&ri);

             
            raxStart(&ri,group->consumers);
            raxSeek(&ri,"^",NULL,0);
            void *arraylen_ptr = addDeferredMultiBulkLength(c);
            size_t arraylen = 0;
            while(raxNext(&ri)) {
                streamConsumer *consumer = ri.data;
                if (raxSize(consumer->pel) == 0) continue;
                addReplyMultiBulkLen(c,2);
                addReplyBulkCBuffer(c,ri.key,ri.key_len);
                addReplyBulkLongLong(c,raxSize(consumer->pel));
                arraylen++;
            }
            setDeferredMultiBulkLength(c,arraylen_ptr,arraylen);
            raxStop(&ri);
        }
    }
     
    else {
        streamConsumer *consumer = consumername ?
                                streamLookupConsumer(group,consumername->ptr,0):
                                NULL;

         
        if (consumername && consumer == NULL) {
            addReplyMultiBulkLen(c,0);
            return;
        }

        rax *pel = consumer ? consumer->pel : group->pel;
        unsigned char startkey[sizeof(streamID)];
        unsigned char endkey[sizeof(streamID)];
        raxIterator ri;
        mstime_t now = mstime();

        streamEncodeID(startkey,&startid);
        streamEncodeID(endkey,&endid);
        raxStart(&ri,pel);
        raxSeek(&ri,">=",startkey,sizeof(startkey));
        void *arraylen_ptr = addDeferredMultiBulkLength(c);
        size_t arraylen = 0;

        while(count && raxNext(&ri) && memcmp(ri.key,endkey,ri.key_len) <= 0) {
            streamNACK *nack = ri.data;

            arraylen++;
            count--;
            addReplyMultiBulkLen(c,4);

             
            streamID id;
            streamDecodeID(ri.key,&id);
            addReplyStreamID(c,&id);

             
            addReplyBulkCBuffer(c,nack->consumer->name,
                                sdslen(nack->consumer->name));

             
            mstime_t elapsed = now - nack->delivery_time;
            if (elapsed < 0) elapsed = 0;
            addReplyLongLong(c,elapsed);

             
            addReplyLongLong(c,nack->delivery_count);
        }
        raxStop(&ri);
        setDeferredMultiBulkLength(c,arraylen_ptr,arraylen);
    }
}