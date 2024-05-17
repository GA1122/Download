 fgetwln(FILE *stream, size_t *lenp)
 {
 	struct filewbuf *fb;
 	wint_t wc;
 	size_t wused = 0;
 
 	 
 	fb = &fb_pool[fb_pool_cur];
 	if (fb->fp != stream && fb->fp != NULL) {
 		fb_pool_cur++;
 		fb_pool_cur %= FILEWBUF_POOL_ITEMS;
 		fb = &fb_pool[fb_pool_cur];
 	}
  	fb->fp = stream;
  
  	while ((wc = fgetwc(stream)) != WEOF) {
		if (!fb->len || wused > fb->len) {
// 		if (!fb->len || wused >= fb->len) {
  			wchar_t *wp;
  
  			if (fb->len)
 				fb->len *= 2;
 			else
 				fb->len = FILEWBUF_INIT_LEN;
 
 			wp = reallocarray(fb->wbuf, fb->len, sizeof(wchar_t));
 			if (wp == NULL) {
 				wused = 0;
 				break;
 			}
 			fb->wbuf = wp;
 		}
 
 		fb->wbuf[wused++] = wc;
 
 		if (wc == L'\n')
 			break;
 	}
 
 	*lenp = wused;
 	return wused ? fb->wbuf : NULL;
 }