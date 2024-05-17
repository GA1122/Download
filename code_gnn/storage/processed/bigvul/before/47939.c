static void assign_register(unsigned long *reg, u64 val, int bytes)
{
	 
	switch (bytes) {
	case 1:
		*(u8 *)reg = (u8)val;
		break;
	case 2:
		*(u16 *)reg = (u16)val;
		break;
	case 4:
		*reg = (u32)val;
		break;	 
	case 8:
		*reg = val;
		break;
	}
}
