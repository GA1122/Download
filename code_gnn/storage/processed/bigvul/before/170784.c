 virtual off64_t getSize() {
 Parcel data, reply;
        data.writeInterfaceToken(
 IMediaHTTPConnection::getInterfaceDescriptor());

        remote()->transact(GET_SIZE, data, &reply);

 int32_t exceptionCode = reply.readExceptionCode();

 if (exceptionCode) {
 return UNKNOWN_ERROR;
 }

 return reply.readInt64();
 }
