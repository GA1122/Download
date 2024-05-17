FrameworkListener::FrameworkListener(const char *socketName) :
 SocketListener(socketName, true, false) {
    init(socketName, false);
}
