fetch_from_evbuffer_socks(struct evbuffer *buf, socks_request_t *req,
                          int log_sockstype, int safe_socks)
{
  char *data;
  ssize_t n_drain;
  size_t datalen, buflen, want_length;
  int res;

  buflen = evbuffer_get_length(buf);
  if (buflen < 2)
    return 0;

  {
     
    struct evbuffer_iovec v;
    int i;
    n_drain = 0;
    i = evbuffer_peek(buf, -1, NULL, &v, 1);
    tor_assert(i == 1);
    data = v.iov_base;
    datalen = v.iov_len;
    want_length = 0;

    res = parse_socks(data, datalen, req, log_sockstype,
                      safe_socks, &n_drain, &want_length);

    if (n_drain < 0)
      evbuffer_drain(buf, evbuffer_get_length(buf));
    else if (n_drain > 0)
      evbuffer_drain(buf, n_drain);

    if (res)
      return res;
  }

   

   
  do {
    int free_data = 0;
    const size_t last_wanted = want_length;
    n_drain = 0;
    data = NULL;
    datalen = inspect_evbuffer(buf, &data, want_length, &free_data, NULL);

    want_length = 0;
    res = parse_socks(data, datalen, req, log_sockstype,
                      safe_socks, &n_drain, &want_length);

    if (free_data)
      tor_free(data);

    if (n_drain < 0)
      evbuffer_drain(buf, evbuffer_get_length(buf));
    else if (n_drain > 0)
      evbuffer_drain(buf, n_drain);

    if (res == 0 && n_drain == 0 && want_length <= last_wanted) {
       
      break;
    }

    buflen = evbuffer_get_length(buf);
  } while (res == 0 && want_length <= buflen && buflen >= 2);

  return res;
}
