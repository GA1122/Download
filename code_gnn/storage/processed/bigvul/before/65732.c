static __be32 nfsd4_decode_bind_conn_to_session(struct nfsd4_compoundargs *argp, struct nfsd4_bind_conn_to_session *bcts)
{
	DECODE_HEAD;

	READ_BUF(NFS4_MAX_SESSIONID_LEN + 8);
	COPYMEM(bcts->sessionid.data, NFS4_MAX_SESSIONID_LEN);
	bcts->dir = be32_to_cpup(p++);
	 
	DECODE_TAIL;
}