struct cred *prepare_exec_creds(void)
{
	struct thread_group_cred *tgcred = NULL;
	struct cred *new;

#ifdef CONFIG_KEYS
	tgcred = kmalloc(sizeof(*tgcred), GFP_KERNEL);
	if (!tgcred)
		return NULL;
#endif

	new = prepare_creds();
	if (!new) {
		kfree(tgcred);
		return new;
	}

#ifdef CONFIG_KEYS
	 
	key_put(new->thread_keyring);
	new->thread_keyring = NULL;

	 
	memcpy(tgcred, new->tgcred, sizeof(struct thread_group_cred));

	atomic_set(&tgcred->usage, 1);
	spin_lock_init(&tgcred->lock);

	 
	key_get(tgcred->session_keyring);
	tgcred->process_keyring = NULL;

	release_tgcred(new);
	new->tgcred = tgcred;
#endif

	return new;
}
