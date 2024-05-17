encode_entry_baggage(struct nfsd3_readdirres *cd, __be32 *p, const char *name,
	     int namlen, u64 ino)
{
	*p++ = xdr_one;				  
	p    = xdr_encode_hyper(p, ino);	  
	p    = xdr_encode_array(p, name, namlen); 

	cd->offset = p;				 
	p = xdr_encode_hyper(p, NFS_OFFSET_MAX); 

	return p;
}
