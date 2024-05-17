static char* allocFromUTF8(const char* in, size_t len)
{
 if (len > 0) {
 if (len == SIZE_MAX) {
 return NULL;
 }
 SharedBuffer* buf = SharedBuffer::alloc(len+1);
        ALOG_ASSERT(buf, "Unable to allocate shared buffer");
 if (buf) {
 char* str = (char*)buf->data();
            memcpy(str, in, len);
            str[len] = 0;
 return str;
 }
 return NULL;
 }

 return getEmptyString();
}
