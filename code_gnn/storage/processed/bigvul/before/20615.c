static void kvm_build_io_pmt(struct kvm *kvm)
{
	unsigned long i, j;

	 
	for (i = 0; i < (sizeof(io_ranges) / sizeof(struct kvm_io_range));
							i++) {
		for (j = io_ranges[i].start;
				j < io_ranges[i].start + io_ranges[i].size;
				j += PAGE_SIZE)
			kvm_set_pmt_entry(kvm, j >> PAGE_SHIFT,
					io_ranges[i].type, 0);
	}

}
