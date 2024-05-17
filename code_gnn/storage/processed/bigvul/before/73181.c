move_buf_to_buf(buf_t *buf_out, buf_t *buf_in, size_t *buf_flushlen)
{
   
  char b[4096];
  size_t cp, len;
  len = *buf_flushlen;
  if (len > buf_in->datalen)
    len = buf_in->datalen;

  cp = len;  
  tor_assert(cp < INT_MAX);
  while (len) {
     
    size_t n = len > sizeof(b) ? sizeof(b) : len;
    fetch_from_buf(b, n, buf_in);
    write_to_buf(b, n, buf_out);
    len -= n;
  }
  *buf_flushlen -= cp;
  return (int)cp;
}
