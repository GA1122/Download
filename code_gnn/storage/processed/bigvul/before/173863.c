 virtual status_t setInputSurface(
            node_id node, OMX_U32 port_index,
 const sp<IGraphicBufferConsumer> &bufferConsumer, MetadataBufferType *type) {
 Parcel data, reply;
        data.writeInterfaceToken(IOMX::getInterfaceDescriptor());
 status_t err;
        data.writeInt32((int32_t)node);
        data.writeInt32(port_index);
        data.writeStrongBinder(IInterface::asBinder(bufferConsumer));

        err = remote()->transact(SET_INPUT_SURFACE, data, &reply);

 if (err != OK) {
            ALOGW("binder transaction failed: %d", err);
 return err;
 }

 int negotiatedType = reply.readInt32();
 if (type != NULL) {
 *type = (MetadataBufferType)negotiatedType;
 }

 return reply.readInt32();
 }
