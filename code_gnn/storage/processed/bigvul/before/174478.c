 virtual status_t setPropertyString(String8 const &name, String8 const &value) const {
 Parcel data, reply;
        data.writeInterfaceToken(IDrm::getInterfaceDescriptor());

        data.writeString8(name);
        data.writeString8(value);
 status_t status = remote()->transact(SET_PROPERTY_STRING, data, &reply);
 if (status != OK) {
 return status;
 }

 return reply.readInt32();
 }
