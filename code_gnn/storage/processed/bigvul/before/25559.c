void __cpuinit per_cpu_trap_init(void)
{
	extern void *vbr_base;

	 

	asm volatile("ldc	%0, vbr"
		     :  
		     : "r" (&vbr_base)
		     : "memory");

	 
	clear_bl_bit();
}
