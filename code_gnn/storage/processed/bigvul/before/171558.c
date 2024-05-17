status_t String8::appendFormat(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

 status_t result = appendFormatV(fmt, args);

    va_end(args);
 return result;
}
