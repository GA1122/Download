 virtual status_t releaseBuffer(int buf, uint64_t frameNumber,
 EGLDisplay display __attribute__((unused)), EGLSyncKHR fence __attribute__((unused)),
 const sp<Fence>& releaseFence) {
 Parcel data, reply;
        data.writeInterfaceToken(IGraphicBufferConsumer::getInterfaceDescriptor());
        data.writeInt32(buf);
        data.writeInt64(frameNumber);
        data.write(*releaseFence);
 status_t result = remote()->transact(RELEASE_BUFFER, data, &reply);
 if (result != NO_ERROR) {
 return result;
 }
 return reply.readInt32();
 }
