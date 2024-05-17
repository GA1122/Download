static void gen_confirm(struct nfs4_client *clp, struct nfsd_net *nn)
{
	__be32 verf[2];

	 
	verf[0] = (__force __be32)get_seconds();
	verf[1] = (__force __be32)nn->clverifier_counter++;
	memcpy(clp->cl_confirm.data, verf, sizeof(clp->cl_confirm.data));
}
