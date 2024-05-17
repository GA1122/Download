 virtual status_t requestBuffer(int bufferIdx, sp<GraphicBuffer>* buf) {
 Parcel data, reply;
        data.writeInterfaceToken(IGraphicBufferProducer::getInterfaceDescriptor());
        data.writeInt32(bufferIdx);
 status_t result =remote()->transact(REQUEST_BUFFER, data, &reply);
 if (result != NO_ERROR) {
 return result;
 }
 bool nonNull = reply.readInt32();
 if (nonNull) {
 *buf = new GraphicBuffer();
            reply.read(**buf);
 }
        result = reply.readInt32();
 return result;
 }
