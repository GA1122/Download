static int generate_and_check_address(struct pt_regs *regs,
				      __u32 opcode,
				      int displacement_not_indexed,
				      int width_shift,
				      __u64 *address)
{
	 

	__u64 base_address, addr;
	int basereg;

	basereg = (opcode >> 20) & 0x3f;
	base_address = regs->regs[basereg];
	if (displacement_not_indexed) {
		__s64 displacement;
		displacement = (opcode >> 10) & 0x3ff;
		displacement = ((displacement << 54) >> 54);  
		addr = (__u64)((__s64)base_address + (displacement << width_shift));
	} else {
		__u64 offset;
		int offsetreg;
		offsetreg = (opcode >> 10) & 0x3f;
		offset = regs->regs[offsetreg];
		addr = base_address + offset;
	}

	 
	if (!address_is_sign_extended(addr)) {
		return -1;
	}

	 
	if (user_mode(regs)) {
		if (addr >= TASK_SIZE) {
			return -1;
		}
		 
	}

	*address = addr;
	return 0;
}