void streamIteratorRemoveEntry(streamIterator *si, streamID *current) {
    unsigned char *lp = si->lp;
    int64_t aux;

     
    int flags = lpGetInteger(si->lp_flags);
    flags |= STREAM_ITEM_FLAG_DELETED;
    lp = lpReplaceInteger(lp,&si->lp_flags,flags);

     
    unsigned char *p = lpFirst(lp);
    aux = lpGetInteger(p);
    lp = lpReplaceInteger(lp,&p,aux-1);
    p = lpNext(lp,p);  
    aux = lpGetInteger(p);
    lp = lpReplaceInteger(lp,&p,aux+1);

     
    si->stream->length--;

     
    streamID start, end;
    if (si->rev) {
        streamDecodeID(si->start_key,&start);
        end = *current;
    } else {
        start = *current;
        streamDecodeID(si->end_key,&end);
    }
    streamIteratorStop(si);
    streamIteratorStart(si,si->stream,&start,&end,si->rev);

     
}
