void do_reserved_inst(unsigned long error_code, struct pt_regs *regs)
{
	 

	unsigned long opcode = 0x6ff4fff0;  
	unsigned long pc, aligned_pc;
	int get_user_error;
	int trapnr = 12;
	int signr = SIGILL;
	char *exception_name = "reserved_instruction";

	pc = regs->pc;
	if ((pc & 3) == 1) {
		 
		aligned_pc = pc & ~3;
		if (!access_ok(VERIFY_READ, aligned_pc, sizeof(unsigned long))) {
			get_user_error = -EFAULT;
		} else {
			get_user_error = __get_user(opcode, (unsigned long *)aligned_pc);
		}
		if (get_user_error >= 0) {
			unsigned long index, shift;
			unsigned long major, minor, combined;
			unsigned long reserved_field;
			reserved_field = opcode & 0xf;  
			major = (opcode >> 26) & 0x3f;
			minor = (opcode >> 16) & 0xf;
			combined = (major << 4) | minor;
			index = major;
			shift = minor << 1;
			if (reserved_field == 0) {
				int opcode_state = (shmedia_opcode_table[index] >> shift) & 0x3;
				switch (opcode_state) {
					case OPCODE_INVALID:
						 
						break;
					case OPCODE_USER_VALID:
						 
						return;
					case OPCODE_PRIV_VALID:
						if (!user_mode(regs)) {
							 
							return;  
						}
						 
						break;
					case OPCODE_CTRL_REG:
						 
						if (!user_mode(regs)) return;
						 
						if (combined == 0x9f) {  
							unsigned long regno = (opcode >> 20) & 0x3f;
							if (regno >= 62) {
								return;
							}
							 
						} else if (combined == 0x1bf) {  
							unsigned long regno = (opcode >> 4) & 0x3f;
							if (regno >= 62) {
								return;
							}
							 
						} else {
							 
						}
						break;
					default:
						 
						break;
				}
			}
			 
		} else {
			 
			trapnr = 87;
			exception_name = "address error (exec)";
			signr = SIGSEGV;
		}
	}

	do_unhandled_exception(trapnr, signr, exception_name, "do_reserved_inst", error_code, regs, current);
}
