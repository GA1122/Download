exsltCryptoBin2Hex (const unsigned char *bin, int binlen,
		    unsigned char *hex, int hexlen) {
    static const char bin2hex[] = { '0', '1', '2', '3',
	'4', '5', '6', '7',
	'8', '9', 'a', 'b',
	'c', 'd', 'e', 'f'
    };

    unsigned char lo, hi;
    int i, pos;
    for (i = 0, pos = 0; (i < binlen && pos < hexlen); i++) {
	lo = bin[i] & 0xf;
	hi = bin[i] >> 4;
	hex[pos++] = bin2hex[hi];
	hex[pos++] = bin2hex[lo];
    }

    hex[pos] = '\0';
}
