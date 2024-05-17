String8::String8(const char16_t* o)
 : mString(allocFromUTF16(o, strlen16(o)))
{
}
