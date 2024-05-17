 virtual status_t getMIMEType(String8 *mimeType) {
 *mimeType = String8("");

 Parcel data, reply;
        data.writeInterfaceToken(
 IMediaHTTPConnection::getInterfaceDescriptor());

        remote()->transact(GET_MIME_TYPE, data, &reply);

 int32_t exceptionCode = reply.readExceptionCode();

 if (exceptionCode) {
 return UNKNOWN_ERROR;
 }

 *mimeType = String8(reply.readString16());

 return OK;
 }
