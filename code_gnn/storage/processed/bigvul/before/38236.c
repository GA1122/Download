static struct sk_filter *__sk_prepare_filter(struct sk_filter *fp,
					     struct sock *sk)
{
	int err;

	fp->bpf_func = NULL;
	fp->jited = 0;

	err = sk_chk_filter(fp->insns, fp->len);
	if (err)
		return ERR_PTR(err);

	 
	bpf_jit_compile(fp);

	 
	if (!fp->jited)
		fp = __sk_migrate_filter(fp, sk);

	return fp;
}
