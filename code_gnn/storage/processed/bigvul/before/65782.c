static __be32 nfsd4_decode_share_deny(struct nfsd4_compoundargs *argp, u32 *x)
{
	__be32 *p;

	READ_BUF(4);
	*x = be32_to_cpup(p++);
	 
	if (*x & ~NFS4_SHARE_DENY_BOTH)
		return nfserr_bad_xdr;
	return nfs_ok;
xdr_error:
	return nfserr_bad_xdr;
}