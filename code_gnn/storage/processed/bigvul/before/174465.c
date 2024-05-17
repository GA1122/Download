 virtual status_t openSession(Vector<uint8_t> &sessionId) {
 Parcel data, reply;
        data.writeInterfaceToken(IDrm::getInterfaceDescriptor());

 status_t status = remote()->transact(OPEN_SESSION, data, &reply);
 if (status != OK) {
 return status;
 }
        readVector(reply, sessionId);

 return reply.readInt32();
 }
