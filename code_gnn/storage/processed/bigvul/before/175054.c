 virtual bool requiresSecureDecoderComponent(
 const char *mime) const {
 Parcel data, reply;
        data.writeInterfaceToken(ICrypto::getInterfaceDescriptor());
        data.writeCString(mime);
        remote()->transact(REQUIRES_SECURE_COMPONENT, data, &reply);

 return reply.readInt32() != 0;
 }
