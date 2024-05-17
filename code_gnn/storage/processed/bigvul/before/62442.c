nfs_printfh(netdissect_options *ndo,
            register const uint32_t *dp, const u_int len)
{
	my_fsid fsid;
	uint32_t ino;
	const char *sfsname = NULL;
	char *spacep;

	if (ndo->ndo_uflag) {
		u_int i;
		char const *sep = "";

		ND_PRINT((ndo, " fh["));
		for (i=0; i<len; i++) {
			ND_PRINT((ndo, "%s%x", sep, dp[i]));
			sep = ":";
		}
		ND_PRINT((ndo, "]"));
		return;
	}

	Parse_fh((const u_char *)dp, len, &fsid, &ino, NULL, &sfsname, 0);

	if (sfsname) {
		 
		char temp[NFSX_V3FHMAX+1];
		u_int stringlen;

		 
		stringlen = len;
		if (stringlen > NFSX_V3FHMAX)
			stringlen = NFSX_V3FHMAX;
		strncpy(temp, sfsname, stringlen);
		temp[stringlen] = '\0';
		 
		spacep = strchr(temp, ' ');
		if (spacep)
			*spacep = '\0';

		ND_PRINT((ndo, " fh %s/", temp));
	} else {
		ND_PRINT((ndo, " fh %d,%d/",
			     fsid.Fsid_dev.Major, fsid.Fsid_dev.Minor));
	}

	if(fsid.Fsid_dev.Minor == 257)
		 
		ND_PRINT((ndo, "%s", fsid.Opaque_Handle));
	else
		ND_PRINT((ndo, "%ld", (long) ino));
}