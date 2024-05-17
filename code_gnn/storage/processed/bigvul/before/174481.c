 virtual status_t unprovisionDevice() {
 Parcel data, reply;
        data.writeInterfaceToken(IDrm::getInterfaceDescriptor());

 status_t status = remote()->transact(UNPROVISION_DEVICE, data, &reply);
 if (status != OK) {
 return status;
 }

 return reply.readInt32();
 }
