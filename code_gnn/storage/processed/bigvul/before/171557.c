status_t String8::append(const char* other, size_t otherLen)
{
 if (bytes() == 0) {
 return setTo(other, otherLen);
 } else if (otherLen == 0) {
 return NO_ERROR;
 }

 return real_append(other, otherLen);
}
