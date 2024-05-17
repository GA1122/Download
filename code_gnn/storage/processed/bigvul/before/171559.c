status_t String8::appendFormatV(const char* fmt, va_list args)
{
 int n, result = NO_ERROR;
    va_list tmp_args;

  
    va_copy(tmp_args, args);
    n = vsnprintf(NULL, 0, fmt, tmp_args);
    va_end(tmp_args);

 if (n != 0) {
 size_t oldLength = length();
 char* buf = lockBuffer(oldLength + n);
 if (buf) {
            vsnprintf(buf + oldLength, n + 1, fmt, args);
 } else {
            result = NO_MEMORY;
 }
 }
 return result;
}
