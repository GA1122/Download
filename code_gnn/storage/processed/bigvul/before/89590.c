MG_INTERNAL void altbuf_append(struct altbuf *ab, char c) {
  if (ab->len < ab->user_buf_size) {
     
    ab->user_buf[ab->len++] = c;
  } else {
     

     
    if (ab->len > 0 && ab->m.len == 0) {
      mbuf_append(&ab->m, ab->user_buf, ab->len);
    }

    mbuf_append(&ab->m, &c, 1);
    ab->len = ab->m.len;
  }
}
