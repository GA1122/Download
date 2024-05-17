int getSocketType(char* name) {
    int type = -1;
    uint i;
    for(i=0; i<sizeof(types)/sizeof(struct socket_type); ++i) {
        if( !strcmp(types[i].name, name) ) {
            type = types[i].type;
            break;
        }
    }
    return type;
}
