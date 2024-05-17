static void copy_clid(struct nfs4_client *target, struct nfs4_client *source)
{
	target->cl_clientid.cl_boot = source->cl_clientid.cl_boot; 
 	target->cl_clientid.cl_id = source->cl_clientid.cl_id; 
 }