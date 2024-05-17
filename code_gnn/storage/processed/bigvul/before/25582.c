static int read_opcode(unsigned long long pc, unsigned long *result_opcode, int from_user_mode)
{
	int get_user_error;
	unsigned long aligned_pc;
	unsigned long opcode;

	if ((pc & 3) == 1) {
		 
		aligned_pc = pc & ~3;
		if (from_user_mode) {
			if (!access_ok(VERIFY_READ, aligned_pc, sizeof(unsigned long))) {
				get_user_error = -EFAULT;
			} else {
				get_user_error = __get_user(opcode, (unsigned long *)aligned_pc);
				*result_opcode = opcode;
			}
			return get_user_error;
		} else {
			 
			*result_opcode = *(unsigned long *) aligned_pc;
			return 0;
		}
	} else if ((pc & 1) == 0) {
		 
		 
		return -EFAULT;
	} else {
		 
		return -EFAULT;
	}
}
