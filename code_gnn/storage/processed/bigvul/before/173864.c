 virtual status_t setInternalOption(
            node_id node,
            OMX_U32 port_index,
 InternalOptionType type,
 const void *optionData,
 size_t size) {
 Parcel data, reply;
        data.writeInterfaceToken(IOMX::getInterfaceDescriptor());
        data.writeInt32((int32_t)node);
        data.writeInt32(port_index);
        data.writeInt64(size);
        data.write(optionData, size);
        data.writeInt32(type);
        remote()->transact(SET_INTERNAL_OPTION, data, &reply);

 return reply.readInt32();
 }
