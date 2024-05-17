void setDeferredMultiBulkLength(client *c, void *node, long length) {
    listNode *ln = (listNode*)node;
    robj *len, *next;

     
    if (node == NULL) return;

    len = listNodeValue(ln);
    len->ptr = sdscatprintf(sdsempty(),"*%ld\r\n",length);
    len->encoding = OBJ_ENCODING_RAW;  
    c->reply_bytes += sdsZmallocSize(len->ptr);
    if (ln->next != NULL) {
        next = listNodeValue(ln->next);

         
        if (next->ptr != NULL) {
            c->reply_bytes -= sdsZmallocSize(len->ptr);
            c->reply_bytes -= getStringObjectSdsUsedMemory(next);
            len->ptr = sdscatlen(len->ptr,next->ptr,sdslen(next->ptr));
            c->reply_bytes += sdsZmallocSize(len->ptr);
            listDelNode(c->reply,ln->next);
        }
    }
    asyncCloseClientOnOutputBufferLimitReached(c);
}
