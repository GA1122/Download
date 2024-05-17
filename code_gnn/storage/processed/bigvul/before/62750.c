static rsRetVal createContext() {
    if (s_context == NULL) {
        DBGPRINTF("imzmq3: creating zctx...");
        zsys_handler_set(NULL);
        s_context = zctx_new();
        
        if (s_context == NULL) {
            errmsg.LogError(0, RS_RET_INVALID_PARAMS,
                            "zctx_new failed: %s",
                            zmq_strerror(errno));
             
            return RS_RET_INVALID_PARAMS;
        }
        DBGPRINTF("success!\n");
        if (runModConf->io_threads > 1) {
            DBGPRINTF("setting io worker threads to %d\n", runModConf->io_threads);
            zctx_set_iothreads(s_context, runModConf->io_threads);
        }
    }
    return RS_RET_OK;
}