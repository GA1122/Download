type_message(struct file *file, png_uint_32 type, const char *what)
  
{
 if (file->global->errors)
 {
      fputs(file->file_name, stderr);
      type_sep(stderr);
      type_name(type, stderr);
      type_sep(stderr);
      fputs(what, stderr);
      putc('\n', stderr);
 }
}
