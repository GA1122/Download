 virtual status_t enableGraphicBuffers(
            node_id node, OMX_U32 port_index, OMX_BOOL enable) {
 Parcel data, reply;
        data.writeInterfaceToken(IOMX::getInterfaceDescriptor());
        data.writeInt32((int32_t)node);
        data.writeInt32(port_index);
        data.writeInt32((uint32_t)enable);
        remote()->transact(ENABLE_GRAPHIC_BUFFERS, data, &reply);

 status_t err = reply.readInt32();
 return err;
 }
