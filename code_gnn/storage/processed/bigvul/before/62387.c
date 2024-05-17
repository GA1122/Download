print_txt_line(netdissect_options *ndo, const char *protoname,
    const char *prefix, const u_char *pptr, u_int idx, u_int len)
{
	u_int startidx;
	u_int linelen;

	startidx = idx;
	while (idx < len) {
		ND_TCHECK(*(pptr+idx));
		if (*(pptr+idx) == '\n') {
			 
			linelen = idx - startidx;
			idx++;
			goto print;
		} else if (*(pptr+idx) == '\r') {
			 
			if ((idx+1) >= len) {
				 
				return (0);
			}
			ND_TCHECK(*(pptr+idx+1));
			if (*(pptr+idx+1) == '\n') {
				 
				linelen = idx - startidx;
				idx += 2;
				goto print;
			}

			 
			return (0);
		} else if (!isascii(*(pptr+idx)) ||
		    (!isprint(*(pptr+idx)) && *(pptr+idx) != '\t')) {
			 
			return (0);
		}
		idx++;
	}

	 
trunc:
	linelen = idx - startidx;
	ND_PRINT((ndo, "%s%.*s[!%s]", prefix, (int)linelen, pptr + startidx,
	    protoname));
	return (0);

print:
	ND_PRINT((ndo, "%s%.*s", prefix, (int)linelen, pptr + startidx));
	return (idx);
}
