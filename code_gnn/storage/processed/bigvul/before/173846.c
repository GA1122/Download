 virtual status_t emptyBuffer(
            node_id node,
            buffer_id buffer,
            OMX_U32 range_offset, OMX_U32 range_length,
            OMX_U32 flags, OMX_TICKS timestamp, int fenceFd) {
 Parcel data, reply;
        data.writeInterfaceToken(IOMX::getInterfaceDescriptor());
        data.writeInt32((int32_t)node);
        data.writeInt32((int32_t)buffer);
        data.writeInt32(range_offset);
        data.writeInt32(range_length);
        data.writeInt32(flags);
        data.writeInt64(timestamp);
        data.writeInt32(fenceFd >= 0);
 if (fenceFd >= 0) {
            data.writeFileDescriptor(fenceFd, true  );
 }
        remote()->transact(EMPTY_BUFFER, data, &reply);

 return reply.readInt32();
 }