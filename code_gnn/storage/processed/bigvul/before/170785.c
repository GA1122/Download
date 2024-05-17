 virtual status_t getUri(String8 *uri) {
 *uri = String8("");

 Parcel data, reply;
        data.writeInterfaceToken(
 IMediaHTTPConnection::getInterfaceDescriptor());

        remote()->transact(GET_URI, data, &reply);

 int32_t exceptionCode = reply.readExceptionCode();

 if (exceptionCode) {
 return UNKNOWN_ERROR;
 }

 *uri = String8(reply.readString16());

 return OK;
 }
