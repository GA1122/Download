        getKeyRequest(Vector<uint8_t> const &sessionId,
 Vector<uint8_t> const &initData,
 String8 const &mimeType, DrmPlugin::KeyType keyType,
 KeyedVector<String8, String8> const &optionalParameters,
 Vector<uint8_t> &request, String8 &defaultUrl,
 DrmPlugin::KeyRequestType *keyRequestType) {
 Parcel data, reply;
        data.writeInterfaceToken(IDrm::getInterfaceDescriptor());

        writeVector(data, sessionId);
        writeVector(data, initData);
        data.writeString8(mimeType);
        data.writeInt32((uint32_t)keyType);

        data.writeInt32(optionalParameters.size());
 for (size_t i = 0; i < optionalParameters.size(); ++i) {
            data.writeString8(optionalParameters.keyAt(i));
            data.writeString8(optionalParameters.valueAt(i));
 }

 status_t status = remote()->transact(GET_KEY_REQUEST, data, &reply);
 if (status != OK) {
 return status;
 }

        readVector(reply, request);
        defaultUrl = reply.readString8();
 *keyRequestType = static_cast<DrmPlugin::KeyRequestType>(reply.readInt32());

 return reply.readInt32();
 }
