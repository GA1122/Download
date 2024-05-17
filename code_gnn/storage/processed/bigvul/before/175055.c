 virtual status_t setMediaDrmSession(const Vector<uint8_t> &sessionId) {
 Parcel data, reply;
        data.writeInterfaceToken(ICrypto::getInterfaceDescriptor());

        writeVector(data, sessionId);
        remote()->transact(SET_MEDIADRM_SESSION, data, &reply);

 return reply.readInt32();
 }
