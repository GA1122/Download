void String8::setPathName(const char* name, size_t len)
{
 char* buf = lockBuffer(len);

    memcpy(buf, name, len);

 if (len > 0 && buf[len-1] == OS_PATH_SEPARATOR)
        len--;

    buf[len] = '\0';

    unlockBuffer(len);
}
