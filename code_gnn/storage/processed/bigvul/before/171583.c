status_t String8::setTo(const char* other, size_t len)
{
 const char *newString = allocFromUTF8(other, len);
 SharedBuffer::bufferFromData(mString)->release();
    mString = newString;
 if (mString) return NO_ERROR;

    mString = getEmptyString();
 return NO_MEMORY;
}
