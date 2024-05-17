read_key_file (struct key2 *key2, const char *file, const unsigned int flags)
{
  struct gc_arena gc = gc_new ();
  struct buffer in;
  int fd, size;
  uint8_t hex_byte[3] = {0, 0, 0};
  const char *error_filename = file;

   
  const unsigned char *cp;
  int hb_index = 0;
  int line_num = 1;
  int line_index = 0;
  int match = 0;

   
  uint8_t* out = (uint8_t*) &key2->keys;
  const int keylen = sizeof (key2->keys);
  int count = 0;

   
# define PARSE_INITIAL        0
# define PARSE_HEAD           1
# define PARSE_DATA           2
# define PARSE_DATA_COMPLETE  3
# define PARSE_FOOT           4
# define PARSE_FINISHED       5
  int state = PARSE_INITIAL;

   
  const int hlen = strlen (static_key_head);
  const int flen = strlen (static_key_foot);
  const int onekeylen = sizeof (key2->keys[0]);

  CLEAR (*key2);

   
  if (flags & RKF_INLINE)  
    {
      size = strlen (file) + 1;
      buf_set_read (&in, (const uint8_t *)file, size);
      error_filename = INLINE_FILE_TAG;
    }
  else  
    {
      in = alloc_buf_gc (2048, &gc);
      fd = platform_open (file, O_RDONLY, 0);
      if (fd == -1)
	msg (M_ERR, "Cannot open file key file '%s'", file);
      size = read (fd, in.data, in.capacity);
      if (size < 0)
	msg (M_FATAL, "Read error on key file ('%s')", file);
      if (size == in.capacity)
	msg (M_FATAL, "Key file ('%s') can be a maximum of %d bytes", file, (int)in.capacity);
      close (fd);
    }

  cp = (unsigned char *)in.data;
  while (size > 0)
    {
      const unsigned char c = *cp;

#if 0
      msg (M_INFO, "char='%c'[%d] s=%d ln=%d li=%d m=%d c=%d",
	   c, (int)c, state, line_num, line_index, match, count);
#endif

      if (c == '\n')
	{
	  line_index = match = 0;
	  ++line_num;	      
	}
      else
	{
	   
	  if (!line_index)
	    {
	       
	      if (state == PARSE_HEAD)
		state = PARSE_DATA;

	       
	      if ((state == PARSE_DATA || state == PARSE_DATA_COMPLETE) && c == '-')
		state = PARSE_FOOT;
	    }

	   
	  if (state == PARSE_INITIAL)
	    {
	      if (line_index < hlen && c == static_key_head[line_index])
		{
		  if (++match == hlen)
		    state = PARSE_HEAD;
		}
	    }

	   
	  if (state == PARSE_FOOT)
	    {
	      if (line_index < flen && c == static_key_foot[line_index])
		{
		  if (++match == flen)
		    state = PARSE_FINISHED;
		}
	    }

	   
	  if (state == PARSE_DATA)
	    {
	      if (isxdigit(c))
		{
		  ASSERT (hb_index >= 0 && hb_index < 2);
		  hex_byte[hb_index++] = c;
		  if (hb_index == 2)
		    {
		      unsigned int u;
		      ASSERT(sscanf((const char *)hex_byte, "%x", &u) == 1);
		      *out++ = u;
		      hb_index = 0;
		      if (++count == keylen)
			state = PARSE_DATA_COMPLETE;
		    }
		}
	      else if (isspace(c))
		;
	      else
		{
		  msg (M_FATAL,
		       (isprint (c) ? printable_char_fmt : unprintable_char_fmt),
		       c, line_num, error_filename, count, onekeylen, keylen);
		}
	    }
	  ++line_index;
	}
      ++cp;
      --size;
    }

   
  key2->n = count / onekeylen;

  ASSERT (key2->n >= 0 && key2->n <= (int) SIZE (key2->keys));

  if (flags & RKF_MUST_SUCCEED)
    {
      if (!key2->n)
	msg (M_FATAL, "Insufficient key material or header text not found in file '%s' (%d/%d/%d bytes found/min/max)",
	     error_filename, count, onekeylen, keylen);

      if (state != PARSE_FINISHED)
	msg (M_FATAL, "Footer text not found in file '%s' (%d/%d/%d bytes found/min/max)",
	     error_filename, count, onekeylen, keylen);
    }

   
  if (!(flags & RKF_INLINE))
    buf_clear (&in);

  if (key2->n)
    warn_if_group_others_accessible (error_filename);

#if 0
   
  {
    int i;
    printf ("KEY READ, n=%d\n", key2->n);
    for (i = 0; i < (int) SIZE (key2->keys); ++i)
      {
	 
	const char *fmt = format_hex_ex ((const uint8_t*)&key2->keys[i],
					 sizeof (key2->keys[i]),
					 0,
					 16,
					 "\n",
					 &gc);
	printf ("[%d]\n%s\n\n", i, fmt);
      }
  }
#endif

   
  gc_free (&gc);
}
