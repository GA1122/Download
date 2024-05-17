int clientsCronHandleTimeout(client *c, mstime_t now_ms) {
    time_t now = now_ms/1000;

    if (server.maxidletime &&
        !(c->flags & CLIENT_SLAVE) &&     
        !(c->flags & CLIENT_MASTER) &&    
        !(c->flags & CLIENT_BLOCKED) &&   
        !(c->flags & CLIENT_PUBSUB) &&    
        (now - c->lastinteraction > server.maxidletime))
    {
        serverLog(LL_VERBOSE,"Closing idle client");
        freeClient(c);
        return 1;
    } else if (c->flags & CLIENT_BLOCKED) {
         

        if (c->bpop.timeout != 0 && c->bpop.timeout < now_ms) {
             
            replyToBlockedClientTimedOut(c);
            unblockClient(c);
        } else if (server.cluster_enabled) {
             
            if (clusterRedirectBlockedClientIfNeeded(c))
                unblockClient(c);
        }
    }
    return 0;
}
