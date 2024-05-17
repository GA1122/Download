String8::String8(const char* o, size_t len)
 : mString(allocFromUTF8(o, len))
{
 if (mString == NULL) {
        mString = getEmptyString();
 }
}
