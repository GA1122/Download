 virtual status_t decrypt(
 const void *inData, size_t size,
 uint32_t streamCTR, uint64_t inputCTR,
 void *outData) {
 Parcel data, reply;
        data.writeInterfaceToken(IHDCP::getInterfaceDescriptor());
        data.writeInt32(size);
        data.write(inData, size);
        data.writeInt32(streamCTR);
        data.writeInt64(inputCTR);
        remote()->transact(HDCP_DECRYPT, data, &reply);

 status_t err = reply.readInt32();

 if (err != OK) {
 return err;
 }

        reply.read(outData, size);

 return err;
 }
