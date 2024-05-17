static void *set_vi_srs_handler(int n, vi_handler_t addr, int srs)
{
	unsigned long handler;
	unsigned long old_handler = vi_handlers[n];
	int srssets = current_cpu_data.srsets;
	u32 *w;
	unsigned char *b;

	BUG_ON(!cpu_has_veic && !cpu_has_vint);

	if (addr == NULL) {
		handler = (unsigned long) do_default_vi;
		srs = 0;
	} else
		handler = (unsigned long) addr;
	vi_handlers[n] = (unsigned long) addr;

	b = (unsigned char *)(ebase + 0x200 + n*VECTORSPACING);

	if (srs >= srssets)
		panic("Shadow register set %d not supported", srs);

	if (cpu_has_veic) {
		if (board_bind_eic_interrupt)
			board_bind_eic_interrupt(n, srs);
	} else if (cpu_has_vint) {
		 
		if (srssets > 1)
			change_c0_srsmap(0xf << n*4, srs << n*4);
	}

	if (srs == 0) {
		 

		extern char except_vec_vi, except_vec_vi_lui;
		extern char except_vec_vi_ori, except_vec_vi_end;
		extern char rollback_except_vec_vi;
		char *vec_start = (cpu_wait == r4k_wait) ?
			&rollback_except_vec_vi : &except_vec_vi;
#ifdef CONFIG_MIPS_MT_SMTC
		 
		extern char except_vec_vi_mori;
		const int mori_offset = &except_vec_vi_mori - vec_start;
#endif  
		const int handler_len = &except_vec_vi_end - vec_start;
		const int lui_offset = &except_vec_vi_lui - vec_start;
		const int ori_offset = &except_vec_vi_ori - vec_start;

		if (handler_len > VECTORSPACING) {
			 
			panic("VECTORSPACING too small");
		}

		memcpy(b, vec_start, handler_len);
#ifdef CONFIG_MIPS_MT_SMTC
		BUG_ON(n > 7);	 

		w = (u32 *)(b + mori_offset);
		*w = (*w & 0xffff0000) | (0x100 << n);
#endif  
		w = (u32 *)(b + lui_offset);
		*w = (*w & 0xffff0000) | (((u32)handler >> 16) & 0xffff);
		w = (u32 *)(b + ori_offset);
		*w = (*w & 0xffff0000) | ((u32)handler & 0xffff);
		local_flush_icache_range((unsigned long)b,
					 (unsigned long)(b+handler_len));
	}
	else {
		 
		w = (u32 *)b;
		*w++ = 0x08000000 | (((u32)handler >> 2) & 0x03fffff);  
		*w = 0;
		local_flush_icache_range((unsigned long)b,
					 (unsigned long)(b+8));
	}

	return (void *)old_handler;
}
