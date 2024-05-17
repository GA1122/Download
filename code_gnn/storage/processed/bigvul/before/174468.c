 virtual status_t queryKeyStatus(Vector<uint8_t> const &sessionId,
 KeyedVector<String8, String8> &infoMap) const {
 Parcel data, reply;
        data.writeInterfaceToken(IDrm::getInterfaceDescriptor());

        writeVector(data, sessionId);
 status_t status = remote()->transact(QUERY_KEY_STATUS, data, &reply);
 if (status != OK) {
 return status;
 }

        infoMap.clear();
 size_t count = reply.readInt32();
 for (size_t i = 0; i < count; i++) {
 String8 key = reply.readString8();
 String8 value = reply.readString8();
            infoMap.add(key, value);
 }
 return reply.readInt32();
 }
