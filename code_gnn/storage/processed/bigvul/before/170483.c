String16 Parcel::readString16() const
{
 size_t len;
 const char16_t* str = readString16Inplace(&len);
 if (str) return String16(str, len);
    ALOGE("Reading a NULL string not supported here.");
 return String16();
}
