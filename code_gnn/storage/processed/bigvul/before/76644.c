static void pdftex_fail(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    fputs("\nError: module writet1", stderr);
    if (cur_file_name)
        fprintf(stderr, " (file %s)", cur_file_name);
    fputs(": ", stderr);
    vsprintf(print_buf, fmt, args);
    fputs(print_buf, stderr);
    fputs("\n ==> Fatal error occurred, the output PS file is not finished!\n", stderr);
    va_end(args);
    exit(-1);
}
