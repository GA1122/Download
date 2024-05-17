status_t String8::append(const String8& other)
{
 const size_t otherLen = other.bytes();
 if (bytes() == 0) {
        setTo(other);
 return NO_ERROR;
 } else if (otherLen == 0) {
 return NO_ERROR;
 }

 return real_append(other.string(), otherLen);
}
