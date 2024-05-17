 virtual status_t removeKeys(Vector<uint8_t> const &keySetId) {
 Parcel data, reply;
        data.writeInterfaceToken(IDrm::getInterfaceDescriptor());

        writeVector(data, keySetId);
 status_t status = remote()->transact(REMOVE_KEYS, data, &reply);
 if (status != OK) {
 return status;
 }

 return reply.readInt32();
 }
