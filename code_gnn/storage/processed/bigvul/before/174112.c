 virtual status_t setSidebandStream(const sp<NativeHandle>& stream) {
 Parcel data, reply;
 status_t result;
        data.writeInterfaceToken(IGraphicBufferProducer::getInterfaceDescriptor());
 if (stream.get()) {
            data.writeInt32(true);
            data.writeNativeHandle(stream->handle());
 } else {
            data.writeInt32(false);
 }
 if ((result = remote()->transact(SET_SIDEBAND_STREAM, data, &reply)) == NO_ERROR) {
            result = reply.readInt32();
 }
 return result;
 }
