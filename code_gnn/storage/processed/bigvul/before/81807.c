size_t streamReplyWithRange(client *c, stream *s, streamID *start, streamID *end, size_t count, int rev, streamCG *group, streamConsumer *consumer, int flags, streamPropInfo *spi) {
    void *arraylen_ptr = NULL;
    size_t arraylen = 0;
    streamIterator si;
    int64_t numfields;
    streamID id;

     
    if (group && streamCompareID(start,&group->last_id) <= 0) {
        return streamReplyWithRangeFromConsumerPEL(c,s,start,end,count,
                                                   consumer);
    }

    if (!(flags & STREAM_RWR_RAWENTRIES))
        arraylen_ptr = addDeferredMultiBulkLength(c);
    streamIteratorStart(&si,s,start,end,rev);
    while(streamIteratorGetID(&si,&id,&numfields)) {
         
        if (group && streamCompareID(&id,&group->last_id) > 0)
            group->last_id = id;

         
        addReplyMultiBulkLen(c,2);
        addReplyStreamID(c,&id);
        addReplyMultiBulkLen(c,numfields*2);

         
        while(numfields--) {
            unsigned char *key, *value;
            int64_t key_len, value_len;
            streamIteratorGetField(&si,&key,&value,&key_len,&value_len);
            addReplyBulkCBuffer(c,key,key_len);
            addReplyBulkCBuffer(c,value,value_len);
        }

         
        if (group && !(flags & STREAM_RWR_NOACK)) {
            unsigned char buf[sizeof(streamID)];
            streamEncodeID(buf,&id);

             
            streamNACK *nack = streamCreateNACK(consumer);
            int retval = 0;
            retval += raxTryInsert(group->pel,buf,sizeof(buf),nack,NULL);
            retval += raxTryInsert(consumer->pel,buf,sizeof(buf),nack,NULL);

             
            if (retval == 0) {
                streamFreeNACK(nack);
                nack = raxFind(group->pel,buf,sizeof(buf));
                serverAssert(nack != raxNotFound);
                raxRemove(nack->consumer->pel,buf,sizeof(buf),NULL);
                 
                nack->consumer = consumer;
                nack->delivery_time = mstime();
                nack->delivery_count++;
                 
                raxInsert(consumer->pel,buf,sizeof(buf),nack,NULL);
            } else if (retval == 1) {
                serverPanic("NACK half-created. Should not be possible.");
            }

             
            if (spi) {
                robj *idarg = createObjectFromStreamID(&id);
                streamPropagateXCLAIM(c,spi->keyname,spi->groupname,idarg,nack);
                decrRefCount(idarg);
            }
        }

        arraylen++;
        if (count && count == arraylen) break;
    }
    streamIteratorStop(&si);
    if (arraylen_ptr) setDeferredMultiBulkLength(c,arraylen_ptr,arraylen);
    return arraylen;
}
