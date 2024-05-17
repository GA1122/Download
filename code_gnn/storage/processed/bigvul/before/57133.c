static void nfs4_init_boot_verifier(const struct nfs_client *clp,
				    nfs4_verifier *bootverf)
{
	__be32 verf[2];

	if (test_bit(NFS4CLNT_PURGE_STATE, &clp->cl_state)) {
		 
		verf[0] = 0;
		verf[1] = cpu_to_be32(NSEC_PER_SEC + 1);
	} else {
		struct nfs_net *nn = net_generic(clp->cl_net, nfs_net_id);
		verf[0] = cpu_to_be32(nn->boot_time.tv_sec);
		verf[1] = cpu_to_be32(nn->boot_time.tv_nsec);
	}
	memcpy(bootverf->data, verf, sizeof(bootverf->data));
}