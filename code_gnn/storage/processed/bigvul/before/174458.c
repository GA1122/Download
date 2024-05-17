 virtual status_t getPropertyByteArray(String8 const &name, Vector<uint8_t> &value) const {
 Parcel data, reply;
        data.writeInterfaceToken(IDrm::getInterfaceDescriptor());

        data.writeString8(name);
 status_t status = remote()->transact(GET_PROPERTY_BYTE_ARRAY, data, &reply);
 if (status != OK) {
 return status;
 }

        readVector(reply, value);
 return reply.readInt32();
 }
