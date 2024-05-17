PseudoramiXDebug(const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    LogVMessageVerb(X_NONE, 3, format, ap);
    va_end(ap);
}
