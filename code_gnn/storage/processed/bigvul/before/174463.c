 virtual status_t initCheck() const {
 Parcel data, reply;
        data.writeInterfaceToken(IDrm::getInterfaceDescriptor());
 status_t status = remote()->transact(INIT_CHECK, data, &reply);
 if (status != OK) {
 return status;
 }

 return reply.readInt32();
 }
