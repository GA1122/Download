decode_rfc5969(char *out, ssize_t len, int pl, const uint8_t *p)
{
 uint8_t ipv4masklen, ipv6prefixlen;
 uint8_t ipv6prefix[16];
 uint8_t br[4];
 int i;
 ssize_t b, bytes = 0;

 if (pl < 22) {
		errno = EINVAL;
 return 0;
 }
 
	ipv4masklen = *p++;
	pl--;
	ipv6prefixlen = *p++;
	pl--;
 
 for (i = 0; i < 16; i++) {
		ipv6prefix[i] = *p++;
		pl--;
 }
 if (out) {
		b= snprintf(out, len,
 "%d %d "
 "%02x%02x:%02x%02x:"
 "%02x%02x:%02x%02x:"
 "%02x%02x:%02x%02x:"
 "%02x%02x:%02x%02x",
		    ipv4masklen, ipv6prefixlen,
		    ipv6prefix[0], ipv6prefix[1], ipv6prefix[2], ipv6prefix[3],
		    ipv6prefix[4], ipv6prefix[5], ipv6prefix[6], ipv6prefix[7],
		    ipv6prefix[8], ipv6prefix[9], ipv6prefix[10],ipv6prefix[11],
		    ipv6prefix[12],ipv6prefix[13],ipv6prefix[14], ipv6prefix[15]
 );
 
		len -= b;
		out += b;
		bytes += b;
 } else {
		bytes += 16 * 2 + 8 + 2 + 1 + 2;
 }

 while (pl >= 4) {
		br[0] = *p++;
		br[1] = *p++;
		br[2] = *p++;
		br[3] = *p++;
		pl -= 4;
 
 if (out) {
			b= snprintf(out, len, " %d.%d.%d.%d",
			    br[0], br[1], br[2], br[3]);
			len -= b;
			out += b;
			bytes += b;
 } else {
			bytes += (4 * 4);
 }
 }
 
 return bytes;
}
