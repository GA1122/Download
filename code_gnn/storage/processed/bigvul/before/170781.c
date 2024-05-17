 virtual bool connect(
 const char *uri, const KeyedVector<String8, String8> *headers) {
 Parcel data, reply;
        data.writeInterfaceToken(
 IMediaHTTPConnection::getInterfaceDescriptor());

 String16 tmp(uri);
        data.writeString16(tmp);

        tmp = String16("");
 if (headers != NULL) {
 for (size_t i = 0; i < headers->size(); ++i) {
 String16 key(headers->keyAt(i).string());
 String16 val(headers->valueAt(i).string());

                tmp.append(key);
                tmp.append(String16(": "));
                tmp.append(val);
                tmp.append(String16("\r\n"));
 }
 }
        data.writeString16(tmp);

        remote()->transact(CONNECT, data, &reply);

 int32_t exceptionCode = reply.readExceptionCode();

 if (exceptionCode) {
 return UNKNOWN_ERROR;
 }

        sp<IBinder> binder = reply.readStrongBinder();
        mMemory = interface_cast<IMemory>(binder);

 return mMemory != NULL;
 }
