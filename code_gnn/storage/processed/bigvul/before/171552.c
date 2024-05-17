String8::String8(const char32_t* o)
 : mString(allocFromUTF32(o, strlen32(o)))
{
}
