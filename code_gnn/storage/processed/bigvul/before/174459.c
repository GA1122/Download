 virtual status_t getPropertyString(String8 const &name, String8 &value) const {
 Parcel data, reply;
        data.writeInterfaceToken(IDrm::getInterfaceDescriptor());

        data.writeString8(name);
 status_t status = remote()->transact(GET_PROPERTY_STRING, data, &reply);
 if (status != OK) {
 return status;
 }

        value = reply.readString8();
 return reply.readInt32();
 }
