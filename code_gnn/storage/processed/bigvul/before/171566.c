String8 String8::formatV(const char* fmt, va_list args)
{
 String8 result;
    result.appendFormatV(fmt, args);
 return result;
}
