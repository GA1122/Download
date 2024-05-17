void RilSapSocket::sOnUnsolicitedResponse(int unsolResponse,
 const void *data,
 size_t datalen) {
 RilSapSocket *sap_socket = getSocketById(RIL_SOCKET_1);
    sap_socket->onUnsolicitedResponse(unsolResponse, (void *)data, datalen);
}
