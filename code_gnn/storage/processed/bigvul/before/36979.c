static void __vmx_enable_intercept_for_msr(unsigned long *msr_bitmap,
						u32 msr, int type)
{
	int f = sizeof(unsigned long);

	if (!cpu_has_vmx_msr_bitmap())
		return;

	 
	if (msr <= 0x1fff) {
		if (type & MSR_TYPE_R)
			 
			__set_bit(msr, msr_bitmap + 0x000 / f);

		if (type & MSR_TYPE_W)
			 
			__set_bit(msr, msr_bitmap + 0x800 / f);

	} else if ((msr >= 0xc0000000) && (msr <= 0xc0001fff)) {
		msr &= 0x1fff;
		if (type & MSR_TYPE_R)
			 
			__set_bit(msr, msr_bitmap + 0x400 / f);

		if (type & MSR_TYPE_W)
			 
			__set_bit(msr, msr_bitmap + 0xc00 / f);

	}
}