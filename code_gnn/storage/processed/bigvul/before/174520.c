decode_rfc3361(int dl, const uint8_t *data)
{
 uint8_t enc;
 unsigned int l;
 char *sip = NULL;
 struct in_addr addr;
 char *p;

 if (dl < 2) {
		errno = EINVAL;
 return 0;
 }

	enc = *data++;
	dl--;
 switch (enc) {
 case 0:
 if ((l = decode_rfc3397(NULL, 0, dl, data)) > 0) {
			sip = xmalloc(l);
			decode_rfc3397(sip, l, dl, data);
 }
 break;
 case 1:
 if (dl == 0 || dl % 4 != 0) {
			errno = EINVAL;
 break;
 }
		addr.s_addr = INADDR_BROADCAST;
		l = ((dl / sizeof(addr.s_addr)) * ((4 * 4) + 1)) + 1;
		sip = p = xmalloc(l);
 while (dl != 0) {
			memcpy(&addr.s_addr, data, sizeof(addr.s_addr));
			data += sizeof(addr.s_addr);
			p += snprintf(p, l - (p - sip), "%s ", inet_ntoa(addr));
			dl -= sizeof(addr.s_addr);
 }
 *--p = '\0';
 break;
 default:
		errno = EINVAL;
 return 0;
 }

 return sip;
}
