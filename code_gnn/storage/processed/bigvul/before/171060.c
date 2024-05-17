FrameworkListener::FrameworkListener(const char *socketName, bool withSeq) :
 SocketListener(socketName, true, withSeq) {
    init(socketName, withSeq);
}
