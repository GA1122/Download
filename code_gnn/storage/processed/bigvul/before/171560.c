String8& String8::appendPath(const char* name)
{
 if (name[0] != OS_PATH_SEPARATOR) {
 if (*name == '\0') {
 return *this;
 }

 size_t len = length();
 if (len == 0) {
            setPathName(name);
 return *this;
 }

 int newlen = strlen(name);

 char* buf = lockBuffer(len+1+newlen);

 if (buf[len-1] != OS_PATH_SEPARATOR)
            buf[len++] = OS_PATH_SEPARATOR;

        memcpy(buf+len, name, newlen+1);
        len += newlen;

        unlockBuffer(len);

 return *this;
 } else {
        setPathName(name);
 return *this;
 }
}
