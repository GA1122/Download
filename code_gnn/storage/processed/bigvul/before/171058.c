void RilSapSocket::sOnUnsolicitedResponse(int unsolResponse,
 const void *data,
 size_t datalen,
        RIL_SOCKET_ID socketId) {
 RilSapSocket *sap_socket = getSocketById(socketId);
 if (sap_socket) {
        sap_socket->onUnsolicitedResponse(unsolResponse, (void *)data, datalen);
 }
}
