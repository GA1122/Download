void xdelCommand(client *c) {
    robj *o;

    if ((o = lookupKeyReadOrReply(c,c->argv[1],shared.czero)) == NULL
        || checkType(c,o,OBJ_STREAM)) return;
    stream *s = o->ptr;

     
    streamID id;
    for (int j = 2; j < c->argc; j++) {
        if (streamParseIDOrReply(c,c->argv[j],&id,0) != C_OK) return;
    }

     
    int deleted = 0;
    for (int j = 2; j < c->argc; j++) {
        streamParseIDOrReply(c,c->argv[j],&id,0);  
        deleted += streamDeleteItem(s,&id);
    }
    signalModifiedKey(c->db,c->argv[1]);
    notifyKeyspaceEvent(NOTIFY_STREAM,"xdel",c->argv[1],c->db->id);
    server.dirty += deleted;
    addReplyLongLong(c,deleted);
}
