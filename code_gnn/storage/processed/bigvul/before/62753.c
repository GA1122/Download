static int getSocketAction(char* name) {
    int action = -1;
    uint i;

     
    for(i=0; i < sizeof(socketActions)/sizeof(socket_action); ++i) {
        if(!strcmp(socketActions[i].name, name)) {
            action = socketActions[i].action;
            break;
        }
    }

     
    if (action == -1) 
        errmsg.LogError(0, NO_ERRCODE, "unknown action %s",name);
    
    return action;
}
