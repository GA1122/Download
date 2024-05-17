RilSapSocket *RilSapSocket::getSocketById(RIL_SOCKET_ID socketId) {
 RilSapSocket *sap_socket;
 RilSapSocketList *current = head;

    RLOGD("Entered getSocketById");
    printList();

 while(NULL != current) {
 if(socketId == current->socket->id) {
            sap_socket = current->socket;
 return sap_socket;
 }
        current = current->next;
 }
 return NULL;
}
