static void show_smap_vma_flags(struct seq_file *m, struct vm_area_struct *vma)
{
	 
	static const char mnemonics[BITS_PER_LONG][2] = {
		 
		[0 ... (BITS_PER_LONG-1)] = "??",

		[ilog2(VM_READ)]	= "rd",
		[ilog2(VM_WRITE)]	= "wr",
		[ilog2(VM_EXEC)]	= "ex",
		[ilog2(VM_SHARED)]	= "sh",
		[ilog2(VM_MAYREAD)]	= "mr",
		[ilog2(VM_MAYWRITE)]	= "mw",
		[ilog2(VM_MAYEXEC)]	= "me",
		[ilog2(VM_MAYSHARE)]	= "ms",
		[ilog2(VM_GROWSDOWN)]	= "gd",
		[ilog2(VM_PFNMAP)]	= "pf",
		[ilog2(VM_DENYWRITE)]	= "dw",
#ifdef CONFIG_X86_INTEL_MPX
		[ilog2(VM_MPX)]		= "mp",
#endif
		[ilog2(VM_LOCKED)]	= "lo",
		[ilog2(VM_IO)]		= "io",
		[ilog2(VM_SEQ_READ)]	= "sr",
		[ilog2(VM_RAND_READ)]	= "rr",
		[ilog2(VM_DONTCOPY)]	= "dc",
		[ilog2(VM_DONTEXPAND)]	= "de",
		[ilog2(VM_ACCOUNT)]	= "ac",
		[ilog2(VM_NORESERVE)]	= "nr",
		[ilog2(VM_HUGETLB)]	= "ht",
		[ilog2(VM_SYNC)]	= "sf",
		[ilog2(VM_ARCH_1)]	= "ar",
		[ilog2(VM_WIPEONFORK)]	= "wf",
		[ilog2(VM_DONTDUMP)]	= "dd",
#ifdef CONFIG_MEM_SOFT_DIRTY
		[ilog2(VM_SOFTDIRTY)]	= "sd",
#endif
		[ilog2(VM_MIXEDMAP)]	= "mm",
		[ilog2(VM_HUGEPAGE)]	= "hg",
		[ilog2(VM_NOHUGEPAGE)]	= "nh",
		[ilog2(VM_MERGEABLE)]	= "mg",
		[ilog2(VM_UFFD_MISSING)]= "um",
		[ilog2(VM_UFFD_WP)]	= "uw",
#ifdef CONFIG_ARCH_HAS_PKEYS
		 
		[ilog2(VM_PKEY_BIT0)]	= "",
		[ilog2(VM_PKEY_BIT1)]	= "",
		[ilog2(VM_PKEY_BIT2)]	= "",
		[ilog2(VM_PKEY_BIT3)]	= "",
#if VM_PKEY_BIT4
		[ilog2(VM_PKEY_BIT4)]	= "",
#endif
#endif  
	};
	size_t i;

	seq_puts(m, "VmFlags: ");
	for (i = 0; i < BITS_PER_LONG; i++) {
		if (!mnemonics[i][0])
			continue;
		if (vma->vm_flags & (1UL << i)) {
			seq_putc(m, mnemonics[i][0]);
			seq_putc(m, mnemonics[i][1]);
			seq_putc(m, ' ');
		}
	}
	seq_putc(m, '\n');
}