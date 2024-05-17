static int getSocketAction(char* name) {
    int action = -1;
    uint i;
    for(i=0; i < sizeof(actions)/sizeof(struct socket_action); ++i) {
        if(!strcmp(actions[i].name, name)) {
            action = actions[i].action;
            break;
        }
    }
    return action;
}
