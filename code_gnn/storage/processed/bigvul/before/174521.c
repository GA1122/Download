decode_rfc3397(char *out, ssize_t len, int pl, const uint8_t *p)
{
 const uint8_t *r, *q = p;
 int count = 0, l, hops;
 uint8_t ltype;

 while (q - p < pl) {
		r = NULL;
		hops = 0;
  
 while ((l = *q++) && q - p < pl) {
			ltype = l & 0xc0;
 if (ltype == 0x80 || ltype == 0x40)
 return 0;
 else if (ltype == 0xc0) {  
				l = (l & 0x3f) << 8;
				l |= *q++;
  
 if (!r)
					r = q;
				hops++;
 if (hops > 255)
 return 0;
				q = p + l;
 if (q - p >= pl)
 return 0;
 } else {
  
				count += l + 1;
 if (out) {
 if ((ssize_t)l + 1 > len) {
						errno = ENOBUFS;
 return -1;
 }
					memcpy(out, q, l);
					out += l;
 *out++ = '.';
					len -= l;
					len--;
 }
				q += l;
 }
 }
  
 if (out)
 *(out - 1) = ' ';
 if (r)
			q = r;
 }

  
 if (out)
 *(out - 1) = 0;

 return count; 
}
