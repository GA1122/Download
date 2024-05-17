void RilSapSocket::printList() {
 RilSapSocketList *current = head;
    RLOGD("Printing socket list");
 while(NULL != current) {
        RLOGD("SocketName:%s",current->socket->name);
        RLOGD("Socket id:%d",current->socket->id);
        current = current->next;
 }
}
