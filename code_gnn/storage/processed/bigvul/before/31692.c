static int intel_snb_pebs_broken(int cpu)
{
	u32 rev = UINT_MAX;  

	switch (cpu_data(cpu).x86_model) {
	case 42:  
		rev = 0x28;
		break;

	case 45:  
		switch (cpu_data(cpu).x86_mask) {
		case 6: rev = 0x618; break;
		case 7: rev = 0x70c; break;
		}
	}

	return (cpu_data(cpu).microcode < rev);
}
