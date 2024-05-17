 virtual status_t queueBuffer(int buf,
 const QueueBufferInput& input, QueueBufferOutput* output) {
 Parcel data, reply;
        data.writeInterfaceToken(IGraphicBufferProducer::getInterfaceDescriptor());
        data.writeInt32(buf);
        data.write(input);
 status_t result = remote()->transact(QUEUE_BUFFER, data, &reply);
 if (result != NO_ERROR) {
 return result;
 }
        memcpy(output, reply.readInplace(sizeof(*output)), sizeof(*output));
        result = reply.readInt32();
 return result;
 }
