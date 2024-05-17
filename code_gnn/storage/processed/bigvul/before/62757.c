static rsRetVal rcv_loop(thrdInfo_t* pThrd){
    size_t          n_items = 0;
    size_t          i;
    int             rv;
    zmq_pollitem_t* items = NULL;
    poller_data*    pollerData = NULL;
    struct lstn_s*  current;
    instanceConf_t* inst;
    DEFiRet;

     
    for (inst = runModConf->root; inst != NULL; inst=inst->next) {
        addListener(inst);
    }
    if (lcnfRoot == NULL) {
        errmsg.LogError(0, NO_ERRCODE, "imzmq3: no listeners were "
                        "started, input not activated.\n");
        ABORT_FINALIZE(RS_RET_NO_RUN);
    }

     
    for(current=lcnfRoot;current!=NULL;current=current->next)
        n_items++;

     
    
      
    CHKmalloc(items = (zmq_pollitem_t*)MALLOC(sizeof(zmq_pollitem_t)*n_items));
    
     
    CHKmalloc(pollerData = (poller_data*)MALLOC(sizeof(poller_data)*n_items));

     
    for(i=0, current = lcnfRoot; current != NULL; current = current->next, i++) {
         
        items[i].socket=current->sock;
        items[i].events = ZMQ_POLLIN;
        
         
        pollerData[i].thread  = pThrd;
        pollerData[i].ruleset = current->pRuleset;
    }

    s_zloop = zloop_new();
    for(i=0; i<n_items; ++i) {
        
        rv = zloop_poller(s_zloop, &items[i], handlePoll, &pollerData[i]);
        if (rv) {
            errmsg.LogError(0, NO_ERRCODE, "imzmq3: zloop_poller failed for item %zu: %s", i, zmq_strerror(errno));
        } 
    }
    DBGPRINTF("imzmq3: zloop_poller starting...");
    zloop_start(s_zloop);   
    zloop_destroy(&s_zloop);
    DBGPRINTF("imzmq3: zloop_poller stopped.");
finalize_it:
    zctx_destroy(&s_context);

    free(items);
    free(pollerData);
    RETiRet;
}
