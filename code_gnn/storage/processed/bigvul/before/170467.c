int32_t Parcel::readExceptionCode() const
{
 int32_t exception_code = readAligned<int32_t>();
 if (exception_code == EX_HAS_REPLY_HEADER) {
 int32_t header_start = dataPosition();
 int32_t header_size = readAligned<int32_t>();
    setDataPosition(header_start + header_size);
 return 0;
 }
 return exception_code;
}
