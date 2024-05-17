void RilSapSocket::initSapSocket(const char *socketName,
        RIL_RadioFunctions *uimFuncs) {

 if (strcmp(socketName, "sap_uim_socket1") == 0) {
 if(!SocketExists(socketName)) {
            addSocketToList(socketName, RIL_SOCKET_1, uimFuncs);
 }
 }

#if (SIM_COUNT >= 2)
 if (strcmp(socketName, "sap_uim_socket2") == 0) {
 if(!SocketExists(socketName)) {
            addSocketToList(socketName, RIL_SOCKET_2, uimFuncs);
 }
 }
#endif

#if (SIM_COUNT >= 3)
 if (strcmp(socketName, "sap_uim_socket3") == 0) {
 if(!SocketExists(socketName)) {
            addSocketToList(socketName, RIL_SOCKET_3, uimFuncs);
 }
 }
#endif

#if (SIM_COUNT >= 4)
 if (strcmp(socketName, "sap_uim_socket4") == 0) {
 if(!SocketExists(socketName)) {
            addSocketToList(socketName, RIL_SOCKET_4, uimFuncs);
 }
 }
#endif
}
