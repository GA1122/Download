 virtual status_t createInputSurface(
            node_id node, OMX_U32 port_index,
            sp<IGraphicBufferProducer> *bufferProducer, MetadataBufferType *type) {
 Parcel data, reply;
 status_t err;
        data.writeInterfaceToken(IOMX::getInterfaceDescriptor());
        data.writeInt32((int32_t)node);
        data.writeInt32(port_index);
        err = remote()->transact(CREATE_INPUT_SURFACE, data, &reply);
 if (err != OK) {
            ALOGW("binder transaction failed: %d", err);
 return err;
 }

 int negotiatedType = reply.readInt32();
 if (type != NULL) {
 *type = (MetadataBufferType)negotiatedType;
 }

        err = reply.readInt32();
 if (err != OK) {
 return err;
 }

 *bufferProducer = IGraphicBufferProducer::asInterface(
                reply.readStrongBinder());

 return err;
 }
