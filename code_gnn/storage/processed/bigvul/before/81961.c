static void getKeySizes(redisReply *keys, int *types,
                        unsigned long long *sizes)
{
    redisReply *reply;
    char *sizecmds[] = {"STRLEN","LLEN","SCARD","HLEN","ZCARD"};
    unsigned int i;

     
    for(i=0;i<keys->elements;i++) {
         
        if(types[i]==TYPE_NONE)
            continue;

        redisAppendCommand(context, "%s %s", sizecmds[types[i]],
            keys->element[i]->str);
    }

     
    for(i=0;i<keys->elements;i++) {
         
        if(types[i] == TYPE_NONE) {
            sizes[i] = 0;
            continue;
        }

         
        if(redisGetReply(context, (void**)&reply)!=REDIS_OK) {
            fprintf(stderr, "Error getting size for key '%s' (%d: %s)\n",
                keys->element[i]->str, context->err, context->errstr);
            exit(1);
        } else if(reply->type != REDIS_REPLY_INTEGER) {
             
            fprintf(stderr,
                "Warning:  %s on '%s' failed (may have changed type)\n",
                 sizecmds[types[i]], keys->element[i]->str);
            sizes[i] = 0;
        } else {
            sizes[i] = reply->integer;
        }

        freeReplyObject(reply);
    }
}
