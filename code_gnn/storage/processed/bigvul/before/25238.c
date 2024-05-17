static int supported_cpu(void)
{
	struct percpu_struct *cpu;
	unsigned long cputype;

	 
	cpu = (struct percpu_struct *)((char *)hwrpb + hwrpb->processor_offset);
	cputype = cpu->type & 0xffffffff;
	 
	return (cputype >= EV67_CPU) && (cputype <= EV69_CPU);
}
