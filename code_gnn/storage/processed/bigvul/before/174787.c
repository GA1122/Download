 virtual status_t dequeueBuffer(int *buf, sp<Fence>* fence, bool async,
 uint32_t w, uint32_t h, uint32_t format, uint32_t usage) {
 Parcel data, reply;
        data.writeInterfaceToken(IGraphicBufferProducer::getInterfaceDescriptor());
        data.writeInt32(async);
        data.writeInt32(w);
        data.writeInt32(h);
        data.writeInt32(format);
        data.writeInt32(usage);
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
