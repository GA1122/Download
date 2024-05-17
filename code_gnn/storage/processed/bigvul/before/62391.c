signed_relts_print(netdissect_options *ndo,
                   int32_t secs)
{
	if (secs < 0) {
		ND_PRINT((ndo, "-"));
		if (secs == INT32_MIN) {
			 
			unsigned_relts_print(ndo, 2147483648U);
		} else {
			 
			unsigned_relts_print(ndo, -secs);
		}
		return;
	}
	unsigned_relts_print(ndo, secs);
}
