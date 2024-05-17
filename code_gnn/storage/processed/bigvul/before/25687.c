static inline void maybe_flush_windows(unsigned int rs1, unsigned int rs2,
				       unsigned int rd)
{
	if(rs2 >= 16 || rs1 >= 16 || rd >= 16) {
		 
		__asm__ __volatile__("save %sp, -0x40, %sp\n\t"
				     "save %sp, -0x40, %sp\n\t"
				     "save %sp, -0x40, %sp\n\t"
				     "save %sp, -0x40, %sp\n\t"
				     "save %sp, -0x40, %sp\n\t"
				     "save %sp, -0x40, %sp\n\t"
				     "save %sp, -0x40, %sp\n\t"
				     "restore; restore; restore; restore;\n\t"
				     "restore; restore; restore;\n\t");
	}
}
