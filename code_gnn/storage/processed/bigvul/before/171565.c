String8 String8::format(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

 String8 result(formatV(fmt, args));

    va_end(args);
 return result;
}
