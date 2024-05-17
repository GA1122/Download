write_byte(struct file *file, int b)
  
{
 if (file->out != NULL)
 {
 if (putc(b, file->out) != b)
 {
         file->write_errno = errno;
         file->status_code |= WRITE_ERROR;
         stop(file, WRITE_ERROR_CODE, "write byte");
 }
 }

 ++(file->write_count);
}
