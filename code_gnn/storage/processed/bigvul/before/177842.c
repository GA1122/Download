  _PUBLIC_ codepoint_t next_codepoint_handle_ext(
                         struct smb_iconv_handle *ic,
                       const char *str, charset_t src_charset,
//                        const char *str, size_t len,
//                        charset_t src_charset,
                         size_t *bytes_consumed)
  {
          
 	smb_iconv_t descriptor;
 	size_t ilen_orig;
 	size_t ilen;
 	size_t olen;
 	char *outbuf;
 
 	if ((str[0] & 0x80) == 0) {
 		*bytes_consumed = 1;
 		return (codepoint_t)str[0];
 	}
 
 	 
       ilen_orig = strnlen(str, 5);
//        ilen_orig = MIN(len, 5);
         ilen = ilen_orig;
  
         descriptor = get_conv_handle(ic, src_charset, CH_UTF16);
 		*bytes_consumed = 1;
 		return INVALID_CODEPOINT;
 	}