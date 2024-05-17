write_key_file (const int nkeys, const char *filename)
{
  struct gc_arena gc = gc_new ();

  int fd, i;
  int nbits = 0;

   
  struct buffer out = alloc_buf_gc (2048, &gc);
  struct buffer nbits_head_text = alloc_buf_gc (128, &gc);

   
  const int bytes_per_line = 16;

   
  fd = platform_open (filename, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);

  if (fd == -1)
    msg (M_ERR, "Cannot open shared secret file '%s' for write", filename);

  buf_printf (&out, "%s\n", static_key_head);

  for (i = 0; i < nkeys; ++i)
    {
      struct key key;
      char* fmt;

       
      generate_key_random (&key, NULL);

       
      fmt = format_hex_ex ((const uint8_t*)&key,
			   sizeof (key),
			   0,
			   bytes_per_line,
			   "\n",
			   &gc);

       
      nbits += sizeof (key) * 8;

       
      buf_printf (&out, "%s\n", fmt);

       
      memset (fmt, 0, strlen(fmt));
      CLEAR (key);
    }

  buf_printf (&out, "%s\n", static_key_foot);

   
  buf_printf (&nbits_head_text, "#\n# %d bit OpenVPN static key\n#\n", nbits);
  buf_write_string_file (&nbits_head_text, filename, fd);

   
  buf_write_string_file (&out, filename, fd);

  if (close (fd))
    msg (M_ERR, "Close error on shared secret file %s", filename);

   
  buf_clear (&out);

   
  gc_free (&gc);

  return nbits;
}
