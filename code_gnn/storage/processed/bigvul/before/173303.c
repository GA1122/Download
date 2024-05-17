log_error(struct file *file, int code, const char *what)
  
{
 if (file->global->errors)
      emit_error(file, code, what);
}
