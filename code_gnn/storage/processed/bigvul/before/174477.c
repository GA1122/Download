 virtual status_t setPropertyByteArray(String8 const &name,
 Vector<uint8_t> const &value) const {
 Parcel data, reply;
        data.writeInterfaceToken(IDrm::getInterfaceDescriptor());

        data.writeString8(name);
        writeVector(data, value);
 status_t status = remote()->transact(SET_PROPERTY_BYTE_ARRAY, data, &reply);
 if (status != OK) {
 return status;
 }

 return reply.readInt32();
 }
