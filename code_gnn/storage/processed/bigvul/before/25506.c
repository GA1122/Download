static int store_updates_sp(struct pt_regs *regs)
{
	unsigned int inst;

	if (get_user(inst, (unsigned int __user *)regs->nip))
		return 0;
	 
	if (((inst >> 16) & 0x1f) != 1)
		return 0;
	 
	switch (inst >> 26) {
	case 37:	 
	case 39:	 
	case 45:	 
	case 53:	 
	case 55:	 
		return 1;
	case 62:	 
		return (inst & 3) == 1;
	case 31:
		 
		switch ((inst >> 1) & 0x3ff) {
		case 181:	 
		case 183:	 
		case 247:	 
		case 439:	 
		case 695:	 
		case 759:	 
			return 1;
		}
	}
	return 0;
}
