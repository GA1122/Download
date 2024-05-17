nfsd4_decode_write(struct nfsd4_compoundargs *argp, struct nfsd4_write *write)
{
	int avail;
	int len;
	DECODE_HEAD;

	status = nfsd4_decode_stateid(argp, &write->wr_stateid);
	if (status)
		return status;
	READ_BUF(16);
	p = xdr_decode_hyper(p, &write->wr_offset);
	write->wr_stable_how = be32_to_cpup(p++);
	if (write->wr_stable_how > NFS_FILE_SYNC)
		goto xdr_error;
	write->wr_buflen = be32_to_cpup(p++);

	 
	avail = (char*)argp->end - (char*)argp->p;
	if (avail + argp->pagelen < write->wr_buflen) {
		dprintk("NFSD: xdr error (%s:%d)\n",
				__FILE__, __LINE__);
		goto xdr_error;
	}
	write->wr_head.iov_base = p;
	write->wr_head.iov_len = avail;
	write->wr_pagelist = argp->pagelist;

	len = XDR_QUADLEN(write->wr_buflen) << 2;
	if (len >= avail) {
		int pages;

		len -= avail;

		pages = len >> PAGE_SHIFT;
		argp->pagelist += pages;
		argp->pagelen -= pages * PAGE_SIZE;
		len -= pages * PAGE_SIZE;

		argp->p = (__be32 *)page_address(argp->pagelist[0]);
		argp->pagelist++;
		argp->end = argp->p + XDR_QUADLEN(PAGE_SIZE);
	}
	argp->p += XDR_QUADLEN(len);

	DECODE_TAIL;
}