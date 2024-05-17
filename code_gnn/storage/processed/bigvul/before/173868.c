 virtual status_t updateGraphicBufferInMeta(
            node_id node, OMX_U32 port_index,
 const sp<GraphicBuffer> &graphicBuffer, buffer_id buffer) {
 Parcel data, reply;
        data.writeInterfaceToken(IOMX::getInterfaceDescriptor());
        data.writeInt32((int32_t)node);
        data.writeInt32(port_index);
        data.write(*graphicBuffer);
        data.writeInt32((int32_t)buffer);
        remote()->transact(UPDATE_GRAPHIC_BUFFER_IN_META, data, &reply);

 status_t err = reply.readInt32();
 return err;
 }
