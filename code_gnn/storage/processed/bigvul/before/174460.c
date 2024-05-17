 virtual status_t getProvisionRequest(String8 const &certType,
 String8 const &certAuthority,
 Vector<uint8_t> &request,
 String8 &defaultUrl) {
 Parcel data, reply;
        data.writeInterfaceToken(IDrm::getInterfaceDescriptor());

        data.writeString8(certType);
        data.writeString8(certAuthority);
 status_t status = remote()->transact(GET_PROVISION_REQUEST, data, &reply);
 if (status != OK) {
 return status;
 }

        readVector(reply, request);
        defaultUrl = reply.readString8();

 return reply.readInt32();
 }
