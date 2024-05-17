RilSapSocket::RilSapSocket(const char *socketName,
        RIL_SOCKET_ID socketId,
        RIL_RadioFunctions *inputUimFuncs):
 RilSocket(socketName, socketId) {
 if (inputUimFuncs) {
        uimFuncs = inputUimFuncs;
 }
}
