zlib_message(struct zlib *zlib, int unexpected)
  
{
 if (zlib->global->errors)
 {
 const char *reason = zlib->z.msg;

 if (reason == NULL)
         reason = "[no message]";

      fputs(zlib->file->file_name, stderr);
      type_sep(stderr);
      type_name(zlib->chunk->chunk_type, stderr);
      fprintf(stderr, ": %szlib error: %d (%s) (%s)\n",
         unexpected ? "unexpected " : "", zlib->rc, zlib_rc(zlib), reason);
 }
}
