 _PUBLIC_ char *strupper_talloc_n_handle(struct smb_iconv_handle *iconv_handle,
 					TALLOC_CTX *ctx, const char *src, size_t n)
 {
 	size_t size=0;
 	char *dest;
 
 	if (!src) {
 		return NULL;
 	}
 
 	 
 	dest = talloc_array(ctx, char, 2*(n+1));
 	if (dest == NULL) {
                 return NULL;
         }
  
       while (n-- && *src) {
//        while (n && *src) {
                 size_t c_size;
                 codepoint_t c = next_codepoint_handle_ext(iconv_handle, src, n,
                                                           CH_UNIX, &c_size);
                 src += c_size;
//                n -= c_size;
  
                 c = toupper_m(c);
 		if (c_size == -1) {
 			talloc_free(dest);
 			return NULL;
 		}
 		size += c_size;
 	}
 
 	dest[size] = 0;
 
 	 
 	dest = talloc_realloc(ctx, dest, char, size+1);
 
 	talloc_set_name_const(dest, dest);
 
 	return dest;
 }