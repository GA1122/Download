void xaddCommand(client *c) {
    streamID id;
    int id_given = 0;  
    long long maxlen = 0;    
    int approx_maxlen = 0;   
    int maxlen_arg_idx = 0;  

     
    int i = 2;  
    for (; i < c->argc; i++) {
        int moreargs = (c->argc-1) - i;  
        char *opt = c->argv[i]->ptr;
        if (opt[0] == '*' && opt[1] == '\0') {
             
            break;
        } else if (!strcasecmp(opt,"maxlen") && moreargs) {
            char *next = c->argv[i+1]->ptr;
             
            if (moreargs >= 2 && next[0] == '~' && next[1] == '\0') {
                approx_maxlen = 1;
                i++;
            }
            if (getLongLongFromObjectOrReply(c,c->argv[i+1],&maxlen,NULL)
                != C_OK) return;
            i++;
            maxlen_arg_idx = i;
        } else {
             
            if (streamParseIDOrReply(c,c->argv[i],&id,0) != C_OK) return;
            id_given = 1;
            break;
        }
    }
    int field_pos = i+1;

     
    if ((c->argc - field_pos) < 2 || ((c->argc-field_pos) % 2) == 1) {
        addReplyError(c,"wrong number of arguments for XADD");
        return;
    }

     
    robj *o;
    stream *s;
    if ((o = streamTypeLookupWriteOrCreate(c,c->argv[1])) == NULL) return;
    s = o->ptr;

     
    if (streamAppendItem(s,c->argv+field_pos,(c->argc-field_pos)/2,
        &id, id_given ? &id : NULL)
        == C_ERR)
    {
        addReplyError(c,"The ID specified in XADD is equal or smaller than the "
                        "target stream top item");
        return;
    }
    addReplyStreamID(c,&id);

    signalModifiedKey(c->db,c->argv[1]);
    notifyKeyspaceEvent(NOTIFY_STREAM,"xadd",c->argv[1],c->db->id);
    server.dirty++;

     
    if (maxlen) {
        if (!streamTrimByLength(s,maxlen,approx_maxlen)) {
             
            robj *zeroobj = createStringObjectFromLongLong(0);
            rewriteClientCommandArgument(c,maxlen_arg_idx,zeroobj);
            decrRefCount(zeroobj);
        } else {
            notifyKeyspaceEvent(NOTIFY_STREAM,"xtrim",c->argv[1],c->db->id);
        }
    }

     
    robj *idarg = createObjectFromStreamID(&id);
    rewriteClientCommandArgument(c,i,idarg);
    decrRefCount(idarg);

     
    if (server.blocked_clients_by_type[BLOCKED_STREAM])
        signalKeyAsReady(c->db, c->argv[1]);
}
