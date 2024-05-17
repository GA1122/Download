status_t String8::setTo(const char* other)
{
 const char *newString = allocFromUTF8(other, strlen(other));
 SharedBuffer::bufferFromData(mString)->release();
    mString = newString;
 if (mString) return NO_ERROR;

    mString = getEmptyString();
 return NO_MEMORY;
}
