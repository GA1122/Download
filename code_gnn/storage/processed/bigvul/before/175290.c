static int findFd(int socket_id) {
 int fd = s_ril_param_socket.fdCommand;
#if (SIM_COUNT >= 2)
 if (socket_id == RIL_SOCKET_2) {
        fd = s_ril_param_socket2.fdCommand;
 }
#if (SIM_COUNT >= 3)
 if (socket_id == RIL_SOCKET_3) {
        fd = s_ril_param_socket3.fdCommand;
 }
#endif
#if (SIM_COUNT >= 4)
 if (socket_id == RIL_SOCKET_4) {
        fd = s_ril_param_socket4.fdCommand;
 }
#endif
#endif
 return fd;
}
