 virtual status_t attachBuffer(int* slot, const sp<GraphicBuffer>& buffer) {
 Parcel data, reply;
        data.writeInterfaceToken(IGraphicBufferConsumer::getInterfaceDescriptor());
        data.write(*buffer.get());
 status_t result = remote()->transact(ATTACH_BUFFER, data, &reply);
 if (result != NO_ERROR) {
 return result;
 }
 *slot = reply.readInt32();
        result = reply.readInt32();
 return result;
 }
