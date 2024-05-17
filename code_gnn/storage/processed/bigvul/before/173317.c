skip_12(struct file *file)
  
{
  
 if (fseek(file->file, 12, SEEK_CUR) != 0)
 {
 if (errno != 0)
         file->read_errno = errno;

      stop(file, UNEXPECTED_ERROR_CODE, "reskip");
 }
}
