void __init signals_init(void)
{
	 
	BUILD_BUG_ON(__ARCH_SI_PREAMBLE_SIZE
		!= offsetof(struct siginfo, _sifields._pad));

	sigqueue_cachep = KMEM_CACHE(sigqueue, SLAB_PANIC);
}
