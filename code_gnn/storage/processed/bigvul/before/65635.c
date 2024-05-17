nfsd4_set_ex_flags(struct nfs4_client *new, struct nfsd4_exchange_id *clid)
{
#ifdef CONFIG_NFSD_PNFS
	new->cl_exchange_flags |= EXCHGID4_FLAG_USE_PNFS_MDS;
#else
	new->cl_exchange_flags |= EXCHGID4_FLAG_USE_NON_PNFS;
#endif

	 
	new->cl_exchange_flags |= EXCHGID4_FLAG_SUPP_MOVED_REFER;

	 
	clid->flags = new->cl_exchange_flags;
}
