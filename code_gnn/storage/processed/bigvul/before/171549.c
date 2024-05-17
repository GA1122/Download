String8::String8(const String16& o)
 : mString(allocFromUTF16(o.string(), o.size()))
{
}
