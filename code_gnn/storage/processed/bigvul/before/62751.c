static rsRetVal createInstance(instanceConf_t** pinst) {
    DEFiRet;
    instanceConf_t* inst;
    CHKmalloc(inst = MALLOC(sizeof(instanceConf_t)));
    
     
    setDefaults(inst);
    
     
    if (runModConf->root == NULL || runModConf->tail == NULL) {
        runModConf->tail = runModConf->root = inst;
    } else {
        runModConf->tail->next = inst;
        runModConf->tail       = inst;
    }
    *pinst = inst;
finalize_it:
    RETiRet;
}