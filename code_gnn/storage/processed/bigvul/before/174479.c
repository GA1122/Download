 virtual status_t sign(Vector<uint8_t> const &sessionId,
 Vector<uint8_t> const &keyId,
 Vector<uint8_t> const &message,
 Vector<uint8_t> &signature) {
 Parcel data, reply;
        data.writeInterfaceToken(IDrm::getInterfaceDescriptor());

        writeVector(data, sessionId);
        writeVector(data, keyId);
        writeVector(data, message);

 status_t status = remote()->transact(SIGN, data, &reply);
 if (status != OK) {
 return status;
 }
        readVector(reply, signature);

 return reply.readInt32();
 }
