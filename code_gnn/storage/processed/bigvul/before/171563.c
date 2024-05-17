ssize_t String8::find(const char* other, size_t start) const
{
 size_t len = size();
 if (start >= len) {
 return -1;
 }
 const char* s = mString+start;
 const char* p = strstr(s, other);
 return p ? p-mString : -1;
}
