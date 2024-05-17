 virtual status_t freeNode(node_id node) {
 Parcel data, reply;
        data.writeInterfaceToken(IOMX::getInterfaceDescriptor());
        data.writeInt32((int32_t)node);
        remote()->transact(FREE_NODE, data, &reply);

 return reply.readInt32();
 }
