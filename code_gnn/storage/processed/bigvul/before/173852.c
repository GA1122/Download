 virtual status_t getExtensionIndex(
            node_id node,
 const char *parameter_name,
            OMX_INDEXTYPE *index) {
 Parcel data, reply;
        data.writeInterfaceToken(IOMX::getInterfaceDescriptor());
        data.writeInt32((int32_t)node);
        data.writeCString(parameter_name);

        remote()->transact(GET_EXTENSION_INDEX, data, &reply);

 status_t err = reply.readInt32();
 if (err == OK) {
 *index = static_cast<OMX_INDEXTYPE>(reply.readInt32());
 } else {
 *index = OMX_IndexComponentStartUnused;
 }

 return err;
 }
