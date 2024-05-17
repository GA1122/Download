status_t Parcel::writeUtf8AsUtf16(const std::string& str) {
 const uint8_t* strData = (uint8_t*)str.data();
 const size_t strLen= str.length();
 const ssize_t utf16Len = utf8_to_utf16_length(strData, strLen);
 if (utf16Len < 0 || utf16Len> std::numeric_limits<int32_t>::max()) {
 return BAD_VALUE;
 }

 status_t err = writeInt32(utf16Len);
 if (err) {
 return err;
 }

 void* dst = writeInplace((utf16Len + 1) * sizeof(char16_t));
 if (!dst) {
 return NO_MEMORY;
 }

    utf8_to_utf16(strData, strLen, (char16_t*)dst);

 return NO_ERROR;
}
