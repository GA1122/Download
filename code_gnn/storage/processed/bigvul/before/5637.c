copy_attr_error (struct error_context *ctx, char const *fmt, ...)
{
  int err = errno;
  va_list ap;

   
  va_start (ap, fmt);
  verror (0, err, fmt, ap);
  va_end (ap);
}
