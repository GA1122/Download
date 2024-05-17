static rsRetVal addListener(instanceConf_t* inst){
     
    void* sock;
    struct lstn_s* newcnfinfo;
    DEFiRet;

    CHKiRet(createSocket(inst, &sock));

     
    CHKmalloc(newcnfinfo=(struct lstn_s*)MALLOC(sizeof(struct lstn_s)));
    newcnfinfo->next = NULL;
    newcnfinfo->sock = sock;
    newcnfinfo->pRuleset = inst->pBindRuleset;
    
     
    if(lcnfRoot == NULL) {
        lcnfRoot = newcnfinfo;
    } 
    if(lcnfLast == NULL) {
        lcnfLast = newcnfinfo;
    } else {
        lcnfLast->next = newcnfinfo;
        lcnfLast = newcnfinfo;
    }

finalize_it:
    RETiRet;
}
