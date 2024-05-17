void jpc_qmfb_split_row(jpc_fix_t *a, int numcols, int parity)
{

	int bufsize = JPC_CEILDIVPOW2(numcols, 1);
	jpc_fix_t splitbuf[QMFB_SPLITBUFSIZE];
	jpc_fix_t *buf = splitbuf;
	register jpc_fix_t *srcptr;
	register jpc_fix_t *dstptr;
	register int n;
	register int m;
	int hstartcol;

	 
	if (bufsize > QMFB_SPLITBUFSIZE) {
		if (!(buf = jas_alloc2(bufsize, sizeof(jpc_fix_t)))) {
			 
			abort();
		}
	}

	if (numcols >= 2) {
		hstartcol = (numcols + 1 - parity) >> 1;
		m = numcols - hstartcol;

		 
		n = m;
		dstptr = buf;
		srcptr = &a[1 - parity];
		while (n-- > 0) {
			*dstptr = *srcptr;
			++dstptr;
			srcptr += 2;
		}
		 
		dstptr = &a[1 - parity];
		srcptr = &a[2 - parity];
		n = numcols - m - (!parity);
		while (n-- > 0) {
			*dstptr = *srcptr;
			++dstptr;
			srcptr += 2;
		}
		 
		dstptr = &a[hstartcol];
		srcptr = buf;
		n = m;
		while (n-- > 0) {
			*dstptr = *srcptr;
			++dstptr;
			++srcptr;
		}
	}

	 
	if (buf != splitbuf) {
		jas_free(buf);
	}

}
