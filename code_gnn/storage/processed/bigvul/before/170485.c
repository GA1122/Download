String8 Parcel::readString8() const
{
 int32_t size = readInt32();
 if (size > 0 && size < INT32_MAX) {
 const char* str = (const char*)readInplace(size+1);
 if (str) return String8(str, size);
 }
 return String8();
}
