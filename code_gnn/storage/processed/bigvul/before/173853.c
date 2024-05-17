 virtual status_t getGraphicBufferUsage(
            node_id node, OMX_U32 port_index, OMX_U32* usage) {
 Parcel data, reply;
        data.writeInterfaceToken(IOMX::getInterfaceDescriptor());
        data.writeInt32((int32_t)node);
        data.writeInt32(port_index);
        remote()->transact(GET_GRAPHIC_BUFFER_USAGE, data, &reply);

 status_t err = reply.readInt32();
 *usage = reply.readInt32();
 return err;
 }
