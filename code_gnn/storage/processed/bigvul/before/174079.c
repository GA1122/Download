 virtual void dump(String8& result, const char* prefix) const {
 Parcel data, reply;
        data.writeInterfaceToken(IGraphicBufferConsumer::getInterfaceDescriptor());
        data.writeString8(result);
        data.writeString8(String8(prefix ? prefix : ""));
        remote()->transact(DUMP, data, &reply);
        reply.readString8();
 }
