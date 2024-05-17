 virtual status_t allocateBuffer(
            node_id node, OMX_U32 port_index, size_t size,
            buffer_id *buffer, void **buffer_data) {
 Parcel data, reply;
        data.writeInterfaceToken(IOMX::getInterfaceDescriptor());
        data.writeInt32((int32_t)node);
        data.writeInt32(port_index);
        data.writeInt64(size);
        remote()->transact(ALLOC_BUFFER, data, &reply);

 status_t err = reply.readInt32();
 if (err != OK) {
 *buffer = 0;

 return err;
 }

 *buffer = (buffer_id)reply.readInt32();
 *buffer_data = (void *)reply.readInt64();

 return err;
 }
