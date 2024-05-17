get_option(const struct dhcp_message *dhcp, uint8_t opt, int *len, int *type)
{
 const uint8_t *p = dhcp->options;
 const uint8_t *e = p + sizeof(dhcp->options);
 uint8_t l, ol = 0;
 uint8_t o = 0;
 uint8_t overl = 0;
 uint8_t *bp = NULL;
 const uint8_t *op = NULL;
 int bl = 0;

  
 while (p + 1 < e) {
		o = *p++;
 if (o == opt) {
 if (op) {
 if (!opt_buffer) {
					opt_buffer = xmalloc(sizeof(*dhcp));
#ifdef DEBUG_MEMORY
					atexit(free_option_buffer);
#endif
 }
 if (!bp) 
					bp = opt_buffer;
				memcpy(bp, op, ol);
				bp += ol;
 }
			ol = (p + *p < e) ? *p : e - (p + 1);
			op = p + 1;
			bl += ol;
 }
 switch (o) {
 case DHO_PAD:
 continue;
 case DHO_END:
 if (overl & 1) {
  
				overl &= ~1;
				p = dhcp->bootfile;
				e = p + sizeof(dhcp->bootfile);
 } else if (overl & 2) {
  
				overl &= ~2;
				p = dhcp->servername;
				e = p + sizeof(dhcp->servername);
 } else
 goto exit;
 break;
 case DHO_OPTIONSOVERLOADED:
  
 if (!overl)
				overl = 0x80 | p[1];
 break;
 }
		l = *p++;
		p += l;
 }

exit:
 if (valid_length(opt, bl, type) == -1) {
		errno = EINVAL;
 return NULL;
 }
 if (len)
 *len = bl;
 if (bp) {
		memcpy(bp, op, ol);
 return (const uint8_t *)opt_buffer;
 }
 if (op)
 return op;
	errno = ENOENT;
 return NULL;
}
