  static char* allocFromUTF32(const char32_t* in, size_t len)
 {
  if (len == 0) {
 
          return getEmptyString();
      }
  
    const ssize_t bytes = utf32_to_utf8_length(in, len);
    if (bytes < 0) {
//     const ssize_t resultStrLen = utf32_to_utf8_length(in, len) + 1;
//     if (resultStrLen < 1) {
          return getEmptyString();
      }
  
    SharedBuffer* buf = SharedBuffer::alloc(bytes+1);
//     SharedBuffer* buf = SharedBuffer::alloc(resultStrLen);
      ALOG_ASSERT(buf, "Unable to allocate shared buffer");
      if (!buf) {
          return getEmptyString();
      }
  
    char* str = (char*) buf->data();
    utf32_to_utf8(in, len, str);
//     char* resultStr = (char*) buf->data();
//     utf32_to_utf8(in, len, resultStr, resultStrLen);
  
    return str;
//     return resultStr;
  }