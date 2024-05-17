void xclaimCommand(client *c) {
    streamCG *group = NULL;
    robj *o = lookupKeyRead(c->db,c->argv[1]);
    long long minidle;  
    long long retrycount = -1;    
    mstime_t deliverytime = -1;   
    int force = 0;
    int justid = 0;

    if (o) {
        if (checkType(c,o,OBJ_STREAM)) return;  
        group = streamLookupCG(o->ptr,c->argv[2]->ptr);
    }

     
    if (o == NULL || group == NULL) {
        addReplyErrorFormat(c,"-NOGROUP No such key '%s' or "
                              "consumer group '%s'", (char*)c->argv[1]->ptr,
                              (char*)c->argv[2]->ptr);
        return;
    }

    if (getLongLongFromObjectOrReply(c,c->argv[4],&minidle,
        "Invalid min-idle-time argument for XCLAIM")
        != C_OK) return;
    if (minidle < 0) minidle = 0;

     
    int j;
    for (j = 4; j < c->argc; j++) {
        streamID id;
        if (streamParseIDOrReply(NULL,c->argv[j],&id,0) != C_OK) break;
    }
    int last_id_arg = j-1;  

     
    time_t now = mstime();
    for (; j < c->argc; j++) {
        int moreargs = (c->argc-1) - j;  
        char *opt = c->argv[j]->ptr;
        if (!strcasecmp(opt,"FORCE")) {
            force = 1;
        } else if (!strcasecmp(opt,"JUSTID")) {
            justid = 1;
        } else if (!strcasecmp(opt,"IDLE") && moreargs) {
            j++;
            if (getLongLongFromObjectOrReply(c,c->argv[j],&deliverytime,
                "Invalid IDLE option argument for XCLAIM")
                != C_OK) return;
            deliverytime = now - deliverytime;
        } else if (!strcasecmp(opt,"TIME") && moreargs) {
            j++;
            if (getLongLongFromObjectOrReply(c,c->argv[j],&deliverytime,
                "Invalid IDLE option argument for XCLAIM")
                != C_OK) return;
        } else if (!strcasecmp(opt,"RETRYCOUNT") && moreargs) {
            j++;
            if (getLongLongFromObjectOrReply(c,c->argv[j],&retrycount,
                "Invalid IDLE option argument for XCLAIM")
                != C_OK) return;
        } else {
            addReplyErrorFormat(c,"Unrecognized XCLAIM option '%s'",opt);
            return;
        }
    }

    if (deliverytime != -1) {
         
        if (deliverytime < 0 || deliverytime > now) deliverytime = now;
    } else {
         
        deliverytime = now;
    }

     
    streamConsumer *consumer = streamLookupConsumer(group,c->argv[3]->ptr,1);
    void *arraylenptr = addDeferredMultiBulkLength(c);
    size_t arraylen = 0;
    for (int j = 5; j <= last_id_arg; j++) {
        streamID id;
        unsigned char buf[sizeof(streamID)];
        if (streamParseIDOrReply(c,c->argv[j],&id,0) != C_OK) return;
        streamEncodeID(buf,&id);

         
        streamNACK *nack = raxFind(group->pel,buf,sizeof(buf));

         
        if (force && nack == raxNotFound) {
            streamIterator myiterator;
            streamIteratorStart(&myiterator,o->ptr,&id,&id,0);
            int64_t numfields;
            int found = 0;
            streamID item_id;
            if (streamIteratorGetID(&myiterator,&item_id,&numfields)) found = 1;
            streamIteratorStop(&myiterator);

             
            if (!found) continue;

             
            nack = streamCreateNACK(NULL);
            raxInsert(group->pel,buf,sizeof(buf),nack,NULL);
        }

        if (nack != raxNotFound) {
             
            if (minidle) {
                mstime_t this_idle = now - nack->delivery_time;
                if (this_idle < minidle) continue;
            }
             
            if (nack->consumer)
                raxRemove(nack->consumer->pel,buf,sizeof(buf),NULL);
             
            nack->consumer = consumer;
            nack->delivery_time = deliverytime;
             
            if (retrycount >= 0) nack->delivery_count = retrycount;
             
            raxInsert(consumer->pel,buf,sizeof(buf),nack,NULL);
             
            if (justid) {
                addReplyStreamID(c,&id);
            } else {
                streamReplyWithRange(c,o->ptr,&id,NULL,1,0,NULL,NULL,
                                     STREAM_RWR_RAWENTRIES,NULL);
            }
            arraylen++;

             
            streamPropagateXCLAIM(c,c->argv[1],c->argv[3],c->argv[j],nack);
            server.dirty++;
        }
    }
    setDeferredMultiBulkLength(c,arraylenptr,arraylen);
    preventCommandPropagation(c);
}
