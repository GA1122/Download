SoftAVC::~SoftAVC() {
    releaseEncoder();
 List<BufferInfo *> &outQueue = getPortQueue(1);
 List<BufferInfo *> &inQueue = getPortQueue(0);
    CHECK(outQueue.empty());
    CHECK(inQueue.empty());
}
