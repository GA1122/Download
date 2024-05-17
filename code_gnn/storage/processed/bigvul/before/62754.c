static int getSocketType(char* name) {
    int type = -1;
    uint i;
    
     
    for(i=0; i<sizeof(socketTypes)/sizeof(socket_type); ++i) {
        if( !strcmp(socketTypes[i].name, name) ) {
            type = socketTypes[i].type;
            break;
        }
    }
    
     
    if (type == -1) 
        errmsg.LogError(0, NO_ERRCODE, "unknown type %s",name);
    
    return type;
}
