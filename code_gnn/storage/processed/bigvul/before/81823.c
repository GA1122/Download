void xtrimCommand(client *c) {
    robj *o;

     
    if ((o = lookupKeyReadOrReply(c,c->argv[1],shared.czero)) == NULL
        || checkType(c,o,OBJ_STREAM)) return;
    stream *s = o->ptr;

     
    int trim_strategy = TRIM_STRATEGY_NONE;
    long long maxlen = 0;    
    int approx_maxlen = 0;   

     
    int i = 2;  
    for (; i < c->argc; i++) {
        int moreargs = (c->argc-1) - i;  
        char *opt = c->argv[i]->ptr;
        if (!strcasecmp(opt,"maxlen") && moreargs) {
            trim_strategy = TRIM_STRATEGY_MAXLEN;
            char *next = c->argv[i+1]->ptr;
             
            if (moreargs >= 2 && next[0] == '~' && next[1] == '\0') {
                approx_maxlen = 1;
                i++;
            }
            if (getLongLongFromObjectOrReply(c,c->argv[i+1],&maxlen,NULL)
                != C_OK) return;
            i++;
        } else {
            addReply(c,shared.syntaxerr);
            return;
        }
    }

     
    int64_t deleted = 0;
    if (trim_strategy == TRIM_STRATEGY_MAXLEN) {
        deleted = streamTrimByLength(s,maxlen,approx_maxlen);
    } else {
        addReplyError(c,"XTRIM called without an option to trim the stream");
        return;
    }

     
    if (deleted) {
        signalModifiedKey(c->db,c->argv[1]);
        notifyKeyspaceEvent(NOTIFY_STREAM,"xtrim",c->argv[1],c->db->id);
        server.dirty += deleted;
    }
    addReplyLongLong(c,deleted);
}
