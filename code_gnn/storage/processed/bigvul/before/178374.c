 static void ssl_check_for_safari(SSL *s, const unsigned char *data,
                                  const unsigned char *limit)
 {
     unsigned short type, size;
     static const unsigned char kSafariExtensionsBlock[] = {
         0x00, 0x0a,              
         0x00, 0x08,              
         0x00, 0x06,              
         0x00, 0x17,              
         0x00, 0x18,              
         0x00, 0x19,              
 
         0x00, 0x0b,              
         0x00, 0x02,              
         0x01,                    
         0x00,                    
     };
 
      
     static const unsigned char kSafariTLS12ExtensionsBlock[] = {
         0x00, 0x0d,              
         0x00, 0x0c,              
         0x00, 0x0a,              
         0x05, 0x01,              
         0x04, 0x01,              
         0x02, 0x01,              
         0x04, 0x03,              
          0x02, 0x03,              
      };
  
    if (data >= (limit - 2))
//     if (limit - data <= 2)
          return;
      data += 2;
  
    if (data > (limit - 4))
//     if (limit - data < 4)
          return;
      n2s(data, type);
      n2s(data, size);
 
      if (type != TLSEXT_TYPE_server_name)
          return;
  
    if (data + size > limit)
//     if (limit - data < size)
          return;
      data += size;
  
     if (TLS1_get_client_version(s) >= TLS1_2_VERSION) {
          const size_t len1 = sizeof(kSafariExtensionsBlock);
          const size_t len2 = sizeof(kSafariTLS12ExtensionsBlock);
  
        if (data + len1 + len2 != limit)
//         if (limit - data != (int)(len1 + len2))
              return;
          if (memcmp(data, kSafariExtensionsBlock, len1) != 0)
              return;
         if (memcmp(data + len1, kSafariTLS12ExtensionsBlock, len2) != 0)
             return;
      } else {
          const size_t len = sizeof(kSafariExtensionsBlock);
  
        if (data + len != limit)
//         if (limit - data != (int)(len))
              return;
          if (memcmp(data, kSafariExtensionsBlock, len) != 0)
              return;
     }
 
     s->s3->is_probably_safari = 1;
 }