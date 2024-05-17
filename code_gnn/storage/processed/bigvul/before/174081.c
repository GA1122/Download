 virtual sp<NativeHandle> getSidebandStream() const {
 Parcel data, reply;
 status_t err;
        data.writeInterfaceToken(IGraphicBufferConsumer::getInterfaceDescriptor());
 if ((err = remote()->transact(GET_SIDEBAND_STREAM, data, &reply)) != NO_ERROR) {
 return NULL;
 }
        sp<NativeHandle> stream;
 if (reply.readInt32()) {
            stream = NativeHandle::create(reply.readNativeHandle(), true);
 }
 return stream;
 }
