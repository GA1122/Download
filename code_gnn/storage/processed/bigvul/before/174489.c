 virtual status_t fillBuffer(node_id node, buffer_id buffer) {
 Parcel data, reply;
        data.writeInterfaceToken(IOMX::getInterfaceDescriptor());
        data.writeInt32((int32_t)node);
        data.writeInt32((int32_t)buffer);
        remote()->transact(FILL_BUFFER, data, &reply);

 return reply.readInt32();
 }
