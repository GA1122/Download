void jpc_qmfb_join_row(jpc_fix_t *a, int numcols, int parity)
{

	int bufsize = JPC_CEILDIVPOW2(numcols, 1);
	jpc_fix_t joinbuf[QMFB_JOINBUFSIZE];
	jpc_fix_t *buf = joinbuf;
	register jpc_fix_t *srcptr;
	register jpc_fix_t *dstptr;
	register int n;
	int hstartcol;

	 
	if (bufsize > QMFB_JOINBUFSIZE) {
		if (!(buf = jas_alloc2(bufsize, sizeof(jpc_fix_t)))) {
			 
			abort();
		}
	}

	hstartcol = (numcols + 1 - parity) >> 1;

	 
	n = hstartcol;
	srcptr = &a[0];
	dstptr = buf;
	while (n-- > 0) {
		*dstptr = *srcptr;
		++srcptr;
		++dstptr;
	}
	 
	srcptr = &a[hstartcol];
	dstptr = &a[1 - parity];
	n = numcols - hstartcol;
	while (n-- > 0) {
		*dstptr = *srcptr;
		dstptr += 2;
		++srcptr;
	}
	 
	srcptr = buf;
	dstptr = &a[parity];
	n = hstartcol;
	while (n-- > 0) {
		*dstptr = *srcptr;
		dstptr += 2;
		++srcptr;
	}

	 
	if (buf != joinbuf) {
		jas_free(buf);
	}

}
