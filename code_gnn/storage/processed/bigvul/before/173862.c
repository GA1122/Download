 virtual status_t setConfig(
            node_id node, OMX_INDEXTYPE index,
 const void *params, size_t size) {
 Parcel data, reply;
        data.writeInterfaceToken(IOMX::getInterfaceDescriptor());
        data.writeInt32((int32_t)node);
        data.writeInt32(index);
        data.writeInt64(size);
        data.write(params, size);
        remote()->transact(SET_CONFIG, data, &reply);

 return reply.readInt32();
 }
