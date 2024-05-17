void RilSapSocket::addSocketToList(const char *socketName, RIL_SOCKET_ID socketid,
        RIL_RadioFunctions *uimFuncs) {
 RilSapSocket* socket = NULL;
 RilSapSocketList *current;

 if(!SocketExists(socketName)) {
        socket = new RilSapSocket(socketName, socketid, uimFuncs);
 RilSapSocketList* listItem = (RilSapSocketList*)malloc(sizeof(RilSapSocketList));
 if (!listItem) {
            RLOGE("addSocketToList: OOM");
 return;
 }
        listItem->socket = socket;
        listItem->next = NULL;

        RLOGD("Adding socket with id: %d", socket->id);

 if(NULL == head) {
            head = listItem;
            head->next = NULL;
 }
 else {
            current = head;
 while(NULL != current->next) {
                current = current->next;
 }
            current->next = listItem;
 }
        socket->socketInit();
 }
}
