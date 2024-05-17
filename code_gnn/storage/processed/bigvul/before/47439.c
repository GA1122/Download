static int __init padlock_init(void)
{
	int ret;
	struct cpuinfo_x86 *c = &cpu_data(0);

	if (!x86_match_cpu(padlock_cpu_id))
		return -ENODEV;

	if (!cpu_has_xcrypt_enabled) {
		printk(KERN_NOTICE PFX "VIA PadLock detected, but not enabled. Hmm, strange...\n");
		return -ENODEV;
	}

	if ((ret = crypto_register_alg(&aes_alg)))
		goto aes_err;

	if ((ret = crypto_register_alg(&ecb_aes_alg)))
		goto ecb_aes_err;

	if ((ret = crypto_register_alg(&cbc_aes_alg)))
		goto cbc_aes_err;

	printk(KERN_NOTICE PFX "Using VIA PadLock ACE for AES algorithm.\n");

	if (c->x86 == 6 && c->x86_model == 15 && c->x86_mask == 2) {
		ecb_fetch_blocks = MAX_ECB_FETCH_BLOCKS;
		cbc_fetch_blocks = MAX_CBC_FETCH_BLOCKS;
		printk(KERN_NOTICE PFX "VIA Nano stepping 2 detected: enabling workaround.\n");
	}

out:
	return ret;

cbc_aes_err:
	crypto_unregister_alg(&ecb_aes_alg);
ecb_aes_err:
	crypto_unregister_alg(&aes_alg);
aes_err:
	printk(KERN_ERR PFX "VIA PadLock AES initialization failed.\n");
	goto out;
}
