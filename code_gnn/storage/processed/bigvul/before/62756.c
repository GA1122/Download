static rsRetVal parseSubscriptions(char* subscribes, sublist** subList){
    char* tok = strtok(subscribes, ",");
    sublist* currentSub;
    sublist* head;
    DEFiRet;

     
    CHKmalloc(*subList = (sublist*)MALLOC(sizeof(sublist)));
    head = *subList;
    head->next = NULL;
    head->subscribe=NULL;
    currentSub=head;
    
    if(tok) {
        head->subscribe=strdup(tok);
        for(tok=strtok(NULL, ","); tok!=NULL;tok=strtok(NULL, ",")) {
            CHKmalloc(currentSub->next = (sublist*)MALLOC(sizeof(sublist)));
            currentSub=currentSub->next;
            currentSub->subscribe=strdup(tok);
            currentSub->next=NULL;
        }
    } else {
         
        head->subscribe=strdup("");
    }
     
    currentSub = head;
    DBGPRINTF("imzmq3: Subscriptions:");
    for(currentSub = head; currentSub != NULL; currentSub=currentSub->next) {
        DBGPRINTF("'%s'", currentSub->subscribe); 
    }
    DBGPRINTF("\n");
    
finalize_it:
    RETiRet;
}
