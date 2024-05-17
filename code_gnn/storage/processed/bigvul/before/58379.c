void __init early_trap_init(void *vectors_base)
{
	unsigned long vectors = (unsigned long)vectors_base;
	extern char __stubs_start[], __stubs_end[];
	extern char __vectors_start[], __vectors_end[];
	extern char __kuser_helper_start[], __kuser_helper_end[];
	int kuser_sz = __kuser_helper_end - __kuser_helper_start;

	vectors_page = vectors_base;

	 
	memcpy((void *)vectors, __vectors_start, __vectors_end - __vectors_start);
	memcpy((void *)vectors + 0x200, __stubs_start, __stubs_end - __stubs_start);
	memcpy((void *)vectors + 0x1000 - kuser_sz, __kuser_helper_start, kuser_sz);

	 
	kuser_get_tls_init(vectors);

	 
	memcpy((void *)(vectors + KERN_SIGRETURN_CODE - CONFIG_VECTORS_BASE),
	       sigreturn_codes, sizeof(sigreturn_codes));

	flush_icache_range(vectors, vectors + PAGE_SIZE);
	modify_domain(DOMAIN_USER, DOMAIN_CLIENT);
}
