bool nfs4_write_need_cache_consistency_data(struct nfs_pgio_header *hdr)
{
	 
	if (hdr->ds_clp != NULL || hdr->dreq != NULL)
		return false;
	 
	return nfs4_have_delegation(hdr->inode, FMODE_READ) == 0;
}
