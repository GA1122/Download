 virtual status_t detachNextBuffer(sp<GraphicBuffer>* outBuffer,
            sp<Fence>* outFence) {
 if (outBuffer == NULL) {
            ALOGE("detachNextBuffer: outBuffer must not be NULL");
 return BAD_VALUE;
 } else if (outFence == NULL) {
            ALOGE("detachNextBuffer: outFence must not be NULL");
 return BAD_VALUE;
 }
 Parcel data, reply;
        data.writeInterfaceToken(IGraphicBufferProducer::getInterfaceDescriptor());
 status_t result = remote()->transact(DETACH_NEXT_BUFFER, data, &reply);
 if (result != NO_ERROR) {
 return result;
 }
        result = reply.readInt32();
 if (result == NO_ERROR) {
 bool nonNull = reply.readInt32();
 if (nonNull) {
 *outBuffer = new GraphicBuffer;
                reply.read(**outBuffer);
 }
            nonNull = reply.readInt32();
 if (nonNull) {
 *outFence = new Fence;
                reply.read(**outFence);
 }
 }
 return result;
 }
