 virtual status_t decrypt(Vector<uint8_t> const &sessionId,
 Vector<uint8_t> const &keyId,
 Vector<uint8_t> const &input,
 Vector<uint8_t> const &iv,
 Vector<uint8_t> &output) {
 Parcel data, reply;
        data.writeInterfaceToken(IDrm::getInterfaceDescriptor());

        writeVector(data, sessionId);
        writeVector(data, keyId);
        writeVector(data, input);
        writeVector(data, iv);

 status_t status = remote()->transact(DECRYPT, data, &reply);
 if (status != OK) {
 return status;
 }
        readVector(reply, output);

 return reply.readInt32();
 }
