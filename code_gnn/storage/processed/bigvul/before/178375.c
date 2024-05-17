 static int ssl_scan_clienthello_custom_tlsext(SSL *s,
                                               const unsigned char *data,
                                               const unsigned char *limit,
                                               int *al)
 {
     unsigned short type, size, len;
      
      if (s->hit || s->cert->srv_ext.meths_count == 0)
          return 1;
  
    if (data >= limit - 2)
//     if (limit - data <= 2)
          return 1;
      n2s(data, len);
  
    if (data > limit - len)
//     if (limit - data < len)
          return 1;
  
    while (data <= limit - 4) {
//     while (limit - data >= 4) {
          n2s(data, type);
          n2s(data, size);
  
        if (data + size > limit)
//         if (limit - data < size)
              return 1;
          if (custom_ext_parse(s, 1   , type, data, size, al) <= 0)
              return 0;
 
         data += size;
     }
 
     return 1;
 }