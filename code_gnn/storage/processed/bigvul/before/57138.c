void nfs4_init_sequence(struct nfs4_sequence_args *args,
			struct nfs4_sequence_res *res, int cache_reply)
{
	args->sa_slot = NULL;
	args->sa_cache_this = cache_reply;
	args->sa_privileged = 0;

	res->sr_slot = NULL;
}