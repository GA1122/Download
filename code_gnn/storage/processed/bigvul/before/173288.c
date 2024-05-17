control_init(struct control *control, struct global *global,
 const char *file_name, const char *out_name)
  
{
 return file_init(&control->file, global, file_name, out_name, control,
      allocate);
}
