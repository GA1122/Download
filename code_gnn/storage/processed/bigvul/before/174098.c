 virtual status_t connect(const sp<IProducerListener>& listener,
 int api, bool producerControlledByApp, QueueBufferOutput* output) {
 Parcel data, reply;
        data.writeInterfaceToken(IGraphicBufferProducer::getInterfaceDescriptor());
 if (listener != NULL) {
            data.writeInt32(1);
            data.writeStrongBinder(IInterface::asBinder(listener));
 } else {
            data.writeInt32(0);
 }
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
