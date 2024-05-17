request_send (const struct request *req, int fd, FILE *warc_tmp)
{
  char *request_string, *p;
  int i, size, write_error;

   
  size = 0;

   
  size += strlen (req->method) + 1 + strlen (req->arg) + 1 + 8 + 2;

  for (i = 0; i < req->hcount; i++)
    {
      struct request_header *hdr = &req->headers[i];
       
      size += strlen (hdr->name) + 2 + strlen (hdr->value) + 2;
    }

   
  size += 3;

  p = request_string = xmalloc (size);

   

  APPEND (p, req->method); *p++ = ' ';
  APPEND (p, req->arg);    *p++ = ' ';
  memcpy (p, "HTTP/1.1\r\n", 10); p += 10;

  for (i = 0; i < req->hcount; i++)
    {
      struct request_header *hdr = &req->headers[i];
      APPEND (p, hdr->name);
      *p++ = ':', *p++ = ' ';
      APPEND (p, hdr->value);
      *p++ = '\r', *p++ = '\n';
    }

  *p++ = '\r', *p++ = '\n', *p++ = '\0';
  assert (p - request_string == size);

#undef APPEND

  DEBUGP (("\n---request begin---\n%s---request end---\n", request_string));

   

  write_error = fd_write (fd, request_string, size - 1, -1);
  if (write_error < 0)
    logprintf (LOG_VERBOSE, _("Failed writing HTTP request: %s.\n"),
               fd_errstr (fd));
  else if (warc_tmp != NULL)
    {
       
      int warc_tmp_written = fwrite (request_string, 1, size - 1, warc_tmp);
      if (warc_tmp_written != size - 1)
        write_error = -2;
    }
  xfree (request_string);
  return write_error;
}
