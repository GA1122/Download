static void sc_asn1_print_utctime(const u8 * buf, size_t buflen)
{
	if (buflen < 8) {
		printf("Error in decoding.\n");
		return;
	}

	print_ascii(buf, 2);		 
	putchar('-');
	print_ascii(buf+2, 2);		 
	putchar('-');
	print_ascii(buf+4, 2);		 
	putchar(' ');
	print_ascii(buf+6, 2);		 
	buf += 8;
	buflen -= 8;
	if (buflen >= 2 && isdigit(buf[0]) && isdigit(buf[1])) {
		putchar(':');
		print_ascii(buf, 2);	 
		buf += 2;
		buflen -= 2;
	}
	if (buflen >= 2 && isdigit(buf[0]) && isdigit(buf[1])) {
		putchar(':');
		print_ascii(buf, 2);	 
		buf += 2;
		buflen -= 2;
	}
	if (buflen >= 4 && '.' == buf[0]) {
		print_ascii(buf, 4);	 
		buf += 4;
		buflen -= 4;
	}

	if (buflen >= 1 && 'Z' == buf[0]) {
		printf(" UTC");
	} else if (buflen >= 5 && ('-' == buf[0] || '+' == buf[0])) {
		putchar(' ');
		print_ascii(buf, 3);	 
		putchar(':');
		print_ascii(buf+3, 2);	 
	}
}