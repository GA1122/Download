static int csnmp_strvbcopy_hexstring(char *dst,  
                                     const struct variable_list *vb,
                                     size_t dst_size) {
  char *buffer_ptr;
  size_t buffer_free;

  dst[0] = 0;

  buffer_ptr = dst;
  buffer_free = dst_size;

  for (size_t i = 0; i < vb->val_len; i++) {
    int status;

    status = snprintf(buffer_ptr, buffer_free, (i == 0) ? "%02x" : ":%02x",
                      (unsigned int)vb->val.bitstring[i]);
    assert(status >= 0);

    if (((size_t)status) >= buffer_free)  
    {
      dst[dst_size - 1] = 0;
      return ENOMEM;
    } else  
    {
      buffer_ptr += (size_t)status;
      buffer_free -= (size_t)status;
    }
  }

  return 0;
}  
