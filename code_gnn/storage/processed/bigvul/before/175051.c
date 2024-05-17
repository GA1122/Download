 virtual bool isCryptoSchemeSupported(const uint8_t uuid[16]) {
 Parcel data, reply;
        data.writeInterfaceToken(ICrypto::getInterfaceDescriptor());
        data.write(uuid, 16);
        remote()->transact(IS_CRYPTO_SUPPORTED, data, &reply);

 return reply.readInt32() != 0;
 }
