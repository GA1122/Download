status_t Parcel::writeString16(const char16_t* str, size_t len)
{
 if (str == NULL) return writeInt32(-1);

 status_t err = writeInt32(len);
 if (err == NO_ERROR) {
        len *= sizeof(char16_t);
 uint8_t* data = (uint8_t*)writeInplace(len+sizeof(char16_t));
 if (data) {
            memcpy(data, str, len);
 *reinterpret_cast<char16_t*>(data+len) = 0;
 return NO_ERROR;
 }
        err = mError;
 }
 return err;
}
