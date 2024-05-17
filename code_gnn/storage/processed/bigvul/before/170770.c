 virtual status_t encrypt(
 const void *inData, size_t size, uint32_t streamCTR,
 uint64_t *outInputCTR, void *outData) {
 Parcel data, reply;
        data.writeInterfaceToken(IHDCP::getInterfaceDescriptor());
        data.writeInt32(size);
        data.write(inData, size);
        data.writeInt32(streamCTR);
        remote()->transact(HDCP_ENCRYPT, data, &reply);

 status_t err = reply.readInt32();

 if (err != OK) {
 *outInputCTR = 0;

 return err;
 }

 *outInputCTR = reply.readInt64();
        reply.read(outData, size);

 return err;
 }
