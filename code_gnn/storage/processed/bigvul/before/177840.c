  static int ldb_dn_escape_internal(char *dst, const char *src, int len)
  {
       const char *p, *s;
//        char c;
         char *d;
       size_t l;
       p = s = src;
//        int i;
         d = dst;
  
       while (p - src < len) {
               p += strcspn(p, ",=\n\r+<>#;\\\" ");
               if (p - src == len)  
                       break;
                
               memcpy(d, s, p - s);
               d += (p - s);  
               switch (*p) {
//        for (i = 0; i < len; i++){
//                c = src[i];
//                switch (c) {
                 case ' ':
                       if (p == src || (p-src)==(len-1)) {
//                        if (i == 0 || i == len - 1) {
                                  
                                 *d++ = '\\';
                               *d++ = *p++;                                     
//                                *d++ = c;
                         } else {
                                  
                               *d++ = *p++;
//                                *d++ = c;
                         }
                         break;
  
 				 
 				*d++ = '\\';
 				*d++ = *p++;					 
 			} else {
 				 
 				*d++ = *p++;
 			}
 			break;
                 case '?':
                          
                         *d++ = '\\';
                       *d++ = *p++;
//                        *d++ = c;
                         break;
  
               default: {
//                case ';':
//                case '\r':
//                case '\n':
//                case '=':
//                case '\0': {
                          
                         unsigned char v;
                         const char *hexbytes = "0123456789ABCDEF";
                       v = *(const unsigned char *)p;
//                        v = (const unsigned char)c;
                         *d++ = '\\';
                         *d++ = hexbytes[v>>4];
                         *d++ = hexbytes[v&0xF];
                       p++;
                         break;
                 }
//                default:
//                        *d++ = c;
                 }
               s = p;  
         }