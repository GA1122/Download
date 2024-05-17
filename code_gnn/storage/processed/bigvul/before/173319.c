stop(struct file *file, int code, const char *what)
  
{
   log_error(file, code, what);

  
 if (file->global->quiet < 2)  
 {
      png_uint_32 type;

 if (file->chunk != NULL)
         type = current_type(file, code);  

 else
         type = file->type;

 if (type)
         type_name(type, stdout);

 else  
         fputs("HEAD", stdout);  

      printf(" ERR %.2x %s ", file->status_code, strcode(code));
  
      emit_string(strerror(file->read_errno), stdout);
      putc(' ', stdout);
      emit_string(strerror(file->write_errno), stdout);
      putc(' ', stdout);
      emit_string(what, stdout);
      putc(' ', stdout);
      fputs(file->file_name, stdout);
      putc('\n', stdout);
 }

   file->status_code |= FILE_ERROR;
   longjmp(file->jmpbuf, code);
}
