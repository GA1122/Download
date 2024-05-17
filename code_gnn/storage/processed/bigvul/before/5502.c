static int DebugPrint(ttfFont *ttf, const char *fmt, ...)
{
    char buf[500];
    va_list args;
    int count;

    if (gs_debug_c('Y')) {
        va_start(args, fmt);
        count = vsnprintf(buf, sizeof(buf), fmt, args);
         
        errwrite(ttf->DebugMem, buf, count);
        va_end(args);
    }
    return 0;
}
