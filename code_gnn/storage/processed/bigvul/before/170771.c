 virtual status_t encryptNative(
 const sp<GraphicBuffer> &graphicBuffer,
 size_t offset, size_t size, uint32_t streamCTR,
 uint64_t *outInputCTR, void *outData) {
 Parcel data, reply;
        data.writeInterfaceToken(IHDCP::getInterfaceDescriptor());
        data.write(*graphicBuffer);
        data.writeInt32(offset);
        data.writeInt32(size);
        data.writeInt32(streamCTR);
        remote()->transact(HDCP_ENCRYPT_NATIVE, data, &reply);

 status_t err = reply.readInt32();

 if (err != OK) {
 *outInputCTR = 0;
 return err;
 }

 *outInputCTR = reply.readInt64();
        reply.read(outData, size);

 return err;
 }
