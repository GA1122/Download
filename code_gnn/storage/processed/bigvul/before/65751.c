nfsd4_decode_layoutcommit(struct nfsd4_compoundargs *argp,
		struct nfsd4_layoutcommit *lcp)
{
	DECODE_HEAD;
	u32 timechange;

	READ_BUF(20);
	p = xdr_decode_hyper(p, &lcp->lc_seg.offset);
	p = xdr_decode_hyper(p, &lcp->lc_seg.length);
	lcp->lc_reclaim = be32_to_cpup(p++);

	status = nfsd4_decode_stateid(argp, &lcp->lc_sid);
	if (status)
		return status;

	READ_BUF(4);
	lcp->lc_newoffset = be32_to_cpup(p++);
	if (lcp->lc_newoffset) {
		READ_BUF(8);
		p = xdr_decode_hyper(p, &lcp->lc_last_wr);
	} else
		lcp->lc_last_wr = 0;
	READ_BUF(4);
	timechange = be32_to_cpup(p++);
	if (timechange) {
		status = nfsd4_decode_time(argp, &lcp->lc_mtime);
		if (status)
			return status;
	} else {
		lcp->lc_mtime.tv_nsec = UTIME_NOW;
	}
	READ_BUF(8);
	lcp->lc_layout_type = be32_to_cpup(p++);

	 
	lcp->lc_up_len = be32_to_cpup(p++);
	if (lcp->lc_up_len > 0) {
		READ_BUF(lcp->lc_up_len);
		READMEM(lcp->lc_up_layout, lcp->lc_up_len);
	}

	DECODE_TAIL;
}