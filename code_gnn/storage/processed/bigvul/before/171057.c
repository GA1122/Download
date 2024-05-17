void RilSapSocket::sOnRequestComplete (RIL_Token t,
        RIL_Errno e,
 void *response,
 size_t responselen) {
 RilSapSocket *sap_socket;
 SapSocketRequest *request = (SapSocketRequest*) t;

    RLOGD("Socket id:%d", request->socketId);

    sap_socket = getSocketById(request->socketId);

 if (sap_socket) {
        sap_socket->onRequestComplete(t,e,response,responselen);
 } else {
        RLOGE("Invalid socket id");
 if (request->curr->payload) {
            free(request->curr->payload);
 }
        free(request->curr);
        free(request);
 }
}
