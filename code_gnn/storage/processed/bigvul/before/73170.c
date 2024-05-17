fetch_from_evbuffer_http(struct evbuffer *buf,
                    char **headers_out, size_t max_headerlen,
                    char **body_out, size_t *body_used, size_t max_bodylen,
                    int force_complete)
{
  struct evbuffer_ptr crlf, content_length;
  size_t headerlen, bodylen, contentlen;

   
  crlf = evbuffer_search(buf, "\r\n\r\n", 4, NULL);
  if (crlf.pos < 0) {
     
    if (evbuffer_get_length(buf) > max_headerlen)
      return -1;  
    return 0;  
  } else if (crlf.pos > (int)max_headerlen) {
    return -1;  
  }

  headerlen = crlf.pos + 4;   
  bodylen = evbuffer_get_length(buf) - headerlen;
  if (bodylen > max_bodylen)
    return -1;  

   
  content_length = evbuffer_search_range(buf, CONTENT_LENGTH,
                                         strlen(CONTENT_LENGTH), NULL, &crlf);

  if (content_length.pos >= 0) {
     
    struct evbuffer_ptr eol;
    char *data = NULL;
    int free_data = 0;
    int n, i;
    n = evbuffer_ptr_set(buf, &content_length, strlen(CONTENT_LENGTH),
                         EVBUFFER_PTR_ADD);
    tor_assert(n == 0);
    eol = evbuffer_search_eol(buf, &content_length, NULL, EVBUFFER_EOL_CRLF);
    tor_assert(eol.pos > content_length.pos);
    tor_assert(eol.pos <= crlf.pos);
    inspect_evbuffer(buf, &data, eol.pos - content_length.pos, &free_data,
                         &content_length);

    i = atoi(data);
    if (free_data)
      tor_free(data);
    if (i < 0) {
      log_warn(LD_PROTOCOL, "Content-Length is less than zero; it looks like "
               "someone is trying to crash us.");
      return -1;
    }
    contentlen = i;
     
    log_debug(LD_HTTP,"Got a contentlen of %d.",(int)contentlen);
    if (bodylen < contentlen) {
      if (!force_complete) {
        log_debug(LD_HTTP,"body not all here yet.");
        return 0;  
      }
    }
    if (bodylen > contentlen) {
      bodylen = contentlen;
      log_debug(LD_HTTP,"bodylen reduced to %d.",(int)bodylen);
    }
  }

  if (headers_out) {
    *headers_out = tor_malloc(headerlen+1);
    evbuffer_remove(buf, *headers_out, headerlen);
    (*headers_out)[headerlen] = '\0';
  }
  if (body_out) {
    tor_assert(headers_out);
    tor_assert(body_used);
    *body_used = bodylen;
    *body_out = tor_malloc(bodylen+1);
    evbuffer_remove(buf, *body_out, bodylen);
    (*body_out)[bodylen] = '\0';
  }
  return 1;
}