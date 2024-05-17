emit_error(struct file *file, int code, const char *what)
  
{
 const char *reason;
 int err = 0;

 switch (code)
 {
 case LIBPNG_WARNING_CODE:   reason = "libpng warning:"; break;
 case LIBPNG_ERROR_CODE:     reason = "libpng error:"; break;
 case ZLIB_ERROR_CODE:       reason = "zlib error:"; break;
 case INVALID_ERROR_CODE:    reason = "invalid"; break;
 case READ_ERROR_CODE:       reason = "read failure:";
                                  err = file->read_errno;
 break;
 case WRITE_ERROR_CODE:      reason = "write error";
                                  err = file->write_errno;
 break;
 case UNEXPECTED_ERROR_CODE: reason = "unexpected error:";
                                  err = file->read_errno;
 if (err == 0)
                                     err = file->write_errno;
 break;
 default:                    reason = "INVALID (internal error):"; break;
 }

 if (err != 0)
      fprintf(stderr, "%s: %s %s [%s]\n", file->file_name, reason, what,
         strerror(err));

 else
      fprintf(stderr, "%s: %s %s\n", file->file_name, reason, what);
}
