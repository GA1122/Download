FrameworkListener::FrameworkListener(int sock) :
 SocketListener(sock, true) {
    init(NULL, false);
}
