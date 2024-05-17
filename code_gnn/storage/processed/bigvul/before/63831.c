my_snprintf( char* str, size_t size, const char* format, ... )
    {
    va_list ap;
    int r;

    va_start( ap, format );
#ifdef HAVE_VSNPRINTF
    r = vsnprintf( str, size, format, ap );
#else  
    r = vsprintf( str, format, ap );
#endif  
    va_end( ap );
    return r;
    }