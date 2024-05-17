 virtual status_t createPlugin(
 const uint8_t uuid[16], const void *opaqueData, size_t opaqueSize) {
 Parcel data, reply;
        data.writeInterfaceToken(ICrypto::getInterfaceDescriptor());
        data.write(uuid, 16);
        data.writeInt32(opaqueSize);

 if (opaqueSize > 0) {
            data.write(opaqueData, opaqueSize);
 }

        remote()->transact(CREATE_PLUGIN, data, &reply);

 return reply.readInt32();
 }
