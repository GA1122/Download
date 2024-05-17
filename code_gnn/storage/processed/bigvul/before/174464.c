 virtual bool isCryptoSchemeSupported(const uint8_t uuid[16], const String8 &mimeType) {
 Parcel data, reply;
        data.writeInterfaceToken(IDrm::getInterfaceDescriptor());
        data.write(uuid, 16);
        data.writeString8(mimeType);
 status_t status = remote()->transact(IS_CRYPTO_SUPPORTED, data, &reply);
 if (status != OK) {
            ALOGE("isCryptoSchemeSupported: binder call failed: %d", status);
 return false;
 }

 return reply.readInt32() != 0;
 }
