exsltCryptoHex2Bin (const unsigned char *hex, int hexlen,
		    unsigned char *bin, int binlen) {
    int i = 0, j = 0;
    unsigned char lo, hi, result, tmp;

    while (i < hexlen && j < binlen) {
	hi = lo = 0;

	tmp = hex[i++];
	if (tmp >= '0' && tmp <= '9')
	    hi = tmp - '0';
	else if (tmp >= 'a' && tmp <= 'f')
	    hi = 10 + (tmp - 'a');

	tmp = hex[i++];
	if (tmp >= '0' && tmp <= '9')
	    lo = tmp - '0';
	else if (tmp >= 'a' && tmp <= 'f')
	    lo = 10 + (tmp - 'a');

	result = hi << 4;
	result += lo;
	bin[j++] = result;
    }

    return j;
}
