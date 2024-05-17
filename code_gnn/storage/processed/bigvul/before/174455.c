 virtual status_t destroyPlugin() {
 Parcel data, reply;
        data.writeInterfaceToken(IDrm::getInterfaceDescriptor());
 status_t status = remote()->transact(DESTROY_PLUGIN, data, &reply);
 if (status != OK) {
 return status;
 }

 return reply.readInt32();
 }
