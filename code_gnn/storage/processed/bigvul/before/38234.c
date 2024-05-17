static struct sk_filter *__sk_migrate_filter(struct sk_filter *fp,
					     struct sock *sk)
{
	struct sock_filter *old_prog;
	struct sk_filter *old_fp;
	int i, err, new_len, old_len = fp->len;

	 
	BUILD_BUG_ON(sizeof(struct sock_filter) !=
		     sizeof(struct sock_filter_int));

	 
	for (i = 0; i < fp->len; i++)
		sk_decode_filter(&fp->insns[i], &fp->insns[i]);

	 
	old_prog = kmemdup(fp->insns, old_len * sizeof(struct sock_filter),
			   GFP_KERNEL);
	if (!old_prog) {
		err = -ENOMEM;
		goto out_err;
	}

	 
	err = sk_convert_filter(old_prog, old_len, NULL, &new_len);
	if (err)
		goto out_err_free;

	 
	old_fp = fp;
	fp = __sk_migrate_realloc(old_fp, sk, sk_filter_size(new_len));
	if (!fp) {
		 
		fp = old_fp;
		err = -ENOMEM;
		goto out_err_free;
	}

	fp->bpf_func = sk_run_filter_int_skb;
	fp->len = new_len;

	 
	err = sk_convert_filter(old_prog, old_len, fp->insnsi, &new_len);
	if (err)
		 
		goto out_err_free;

	kfree(old_prog);
	return fp;

out_err_free:
	kfree(old_prog);
out_err:
	 
	if (sk != NULL)
		sk_filter_uncharge(sk, fp);
	else
		kfree(fp);
	return ERR_PTR(err);
}
