static unsigned int irq_mod_ticks(unsigned int usecs, unsigned int resolution)
{
	if (usecs == 0)
		return 0;
	if (usecs < resolution)
		return 1;  
	return usecs / resolution;
}
