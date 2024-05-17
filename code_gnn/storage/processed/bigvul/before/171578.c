bool String8::removeAll(const char* other) {
 ssize_t index = find(other);
 if (index < 0) return false;

 char* buf = lockBuffer(size());
 if (!buf) return false;  

 size_t skip = strlen(other);
 size_t len = size();
 size_t tail = index;
 while (size_t(index) < len) {
 ssize_t next = find(other, index + skip);
 if (next < 0) {
            next = len;
 }

        memmove(buf + tail, buf + index + skip, next - index - skip);
        tail += next - index - skip;
        index = next;
 }
    unlockBuffer(tail);
 return true;
}
