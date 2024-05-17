 virtual status_t setListener(const sp<IDrmClient>& listener) {
 Parcel data, reply;
        data.writeInterfaceToken(IDrm::getInterfaceDescriptor());
        data.writeStrongBinder(IInterface::asBinder(listener));
 status_t status = remote()->transact(SET_LISTENER, data, &reply);
 if (status != OK) {
 return status;
 }
 return reply.readInt32();
 }
