 virtual status_t provideProvisionResponse(Vector<uint8_t> const &response,
 Vector<uint8_t> &certificate,
 Vector<uint8_t> &wrappedKey) {
 Parcel data, reply;
        data.writeInterfaceToken(IDrm::getInterfaceDescriptor());

        writeVector(data, response);
 status_t status = remote()->transact(PROVIDE_PROVISION_RESPONSE, data, &reply);
 if (status != OK) {
 return status;
 }

        readVector(reply, certificate);
        readVector(reply, wrappedKey);

 return reply.readInt32();
 }
