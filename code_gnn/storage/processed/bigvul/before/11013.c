deinit_stringbuf (struct stringbuf *sb)
{
  xfree (sb->buf);
  sb->buf = NULL;
  sb->out_of_core = 1;  
}
