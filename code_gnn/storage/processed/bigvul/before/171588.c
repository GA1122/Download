void String8::toLower(size_t start, size_t length)
{
 const size_t len = size();
 if (start >= len) {
 return;
 }
 if (start+length > len) {
        length = len-start;
 }
 char* buf = lockBuffer(len);
    buf += start;
 while (length > 0) {
 *buf = tolower(*buf);
        buf++;
        length--;
 }
    unlockBuffer(len);
}
