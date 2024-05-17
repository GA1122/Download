 virtual status_t fillBuffer(node_id node, buffer_id buffer, int fenceFd) {
 Parcel data, reply;
        data.writeInterfaceToken(IOMX::getInterfaceDescriptor());
        data.writeInt32((int32_t)node);
        data.writeInt32((int32_t)buffer);
        data.writeInt32(fenceFd >= 0);
 if (fenceFd >= 0) {
            data.writeFileDescriptor(fenceFd, true  );
 }
        remote()->transact(FILL_BUFFER, data, &reply);

 return reply.readInt32();
 }
