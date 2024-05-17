 virtual status_t dequeueBuffer(int *buf, sp<Fence>* fence, bool async,
 uint32_t width, uint32_t height, PixelFormat format,
 uint32_t usage) {
 Parcel data, reply;
        data.writeInterfaceToken(IGraphicBufferProducer::getInterfaceDescriptor());
        data.writeInt32(static_cast<int32_t>(async));
        data.writeUint32(width);
        data.writeUint32(height);
        data.writeInt32(static_cast<int32_t>(format));
        data.writeUint32(usage);
 status_t result = remote()->transact(DEQUEUE_BUFFER, data, &reply);
 if (result != NO_ERROR) {
 return result;
 }
 *buf = reply.readInt32();
 bool nonNull = reply.readInt32();
 if (nonNull) {
 *fence = new Fence();
            reply.read(**fence);
 }
        result = reply.readInt32();
 return result;
 }
