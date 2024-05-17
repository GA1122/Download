void teecore_init_pub_ram(void)
{
	vaddr_t s;
	vaddr_t e;

	 
	core_mmu_get_mem_by_type(MEM_AREA_NSEC_SHM, &s, &e);

	if (s >= e || s & SMALL_PAGE_MASK || e & SMALL_PAGE_MASK)
		panic("invalid PUB RAM");

	 
	if (!tee_vbuf_is_non_sec(s, e - s))
		panic("PUB RAM is not non-secure");

#ifdef CFG_PL310
	 
	tee_l2cc_store_mutex_boot_pa(virt_to_phys((void *)s));
	s += sizeof(uint32_t);			 
	s =  ROUNDUP(s, SMALL_PAGE_SIZE);	 
#endif

	default_nsec_shm_paddr = virt_to_phys((void *)s);
	default_nsec_shm_size = e - s;
}
