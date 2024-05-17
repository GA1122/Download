IW_IMPL(void) iw_utf8_to_ascii(const char *src, char *dst, int dstlen)
{
	struct iw_utf8cvt_struct s;
	int sp;
	unsigned char c;
	unsigned int pending_char;
	int bytes_expected;

	s.dst = dst;
	s.dstlen = dstlen;
	s.dp = 0;
	pending_char=0;
	bytes_expected=0;

	for(sp=0;src[sp];sp++) {
		c = (unsigned char)src[sp];
		if(c<128) {  
			utf8cvt_emitoctet(&s,c);
			bytes_expected=0;
		}
		else if(c<0xc0) {  
			if(bytes_expected>0) {
				pending_char = (pending_char<<6)|(c&0x3f);
				bytes_expected--;
				if(bytes_expected<1) {
					utf8cvt_emitunichar(&s,pending_char);
				}
			}
		}
		else if(c<0xe0) {  
			pending_char = c&0x1f;
			bytes_expected=1;
		}
		else if(c<0xf0) {  
			pending_char = c&0x0f;
			bytes_expected=2;
		}
		else if(c<0xf8) {  
			pending_char = c&0x07;
			bytes_expected=3;
		}
	}
	dst[s.dp] = '\0';
}