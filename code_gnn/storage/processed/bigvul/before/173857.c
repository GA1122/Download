 virtual bool livesLocally(node_id node, pid_t pid) {
 Parcel data, reply;
        data.writeInterfaceToken(IOMX::getInterfaceDescriptor());
        data.writeInt32((int32_t)node);
        data.writeInt32(pid);
        remote()->transact(LIVES_LOCALLY, data, &reply);

 return reply.readInt32() != 0;
 }
