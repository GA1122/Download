 virtual status_t connect(const sp<IBinder>& token,
 int api, bool producerControlledByApp, QueueBufferOutput* output) {
 Parcel data, reply;
        data.writeInterfaceToken(IGraphicBufferProducer::getInterfaceDescriptor());
        data.writeStrongBinder(token);
        data.writeInt32(api);
        data.writeInt32(producerControlledByApp);
 status_t result = remote()->transact(CONNECT, data, &reply);
 if (result != NO_ERROR) {
 return result;
 }
        memcpy(output, reply.readInplace(sizeof(*output)), sizeof(*output));
        result = reply.readInt32();
 return result;
 }
