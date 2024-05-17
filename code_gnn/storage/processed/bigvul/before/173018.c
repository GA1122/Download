display_init(struct display *dp)
  
{
   memset(dp, 0, sizeof *dp);
   dp->options = WARNINGS;  
   dp->filename = NULL;
   dp->operation = NULL;
   dp->original_pp = NULL;
   dp->original_ip = NULL;
   dp->original_rows = NULL;
   dp->read_pp = NULL;
   dp->read_ip = NULL;
   buffer_init(&dp->original_file);

#  ifdef PNG_WRITE_SUPPORTED
      dp->write_pp = NULL;
      buffer_init(&dp->written_file);
#  endif
}
