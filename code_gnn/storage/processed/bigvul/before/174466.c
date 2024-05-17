 virtual status_t provideKeyResponse(Vector<uint8_t> const &sessionId,
 Vector<uint8_t> const &response,
 Vector<uint8_t> &keySetId) {
 Parcel data, reply;
        data.writeInterfaceToken(IDrm::getInterfaceDescriptor());
        writeVector(data, sessionId);
        writeVector(data, response);

 status_t status = remote()->transact(PROVIDE_KEY_RESPONSE, data, &reply);
 if (status != OK) {
 return status;
 }

        readVector(reply, keySetId);

 return reply.readInt32();
 }
