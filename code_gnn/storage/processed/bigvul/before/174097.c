 virtual void cancelBuffer(int buf, const sp<Fence>& fence) {
 Parcel data, reply;
        data.writeInterfaceToken(IGraphicBufferProducer::getInterfaceDescriptor());
        data.writeInt32(buf);
        data.write(*fence.get());
        remote()->transact(CANCEL_BUFFER, data, &reply);
 }
