static inline int handle_delayslot(struct pt_regs *regs,
				   insn_size_t old_instruction,
				   struct mem_access *ma)
{
	insn_size_t instruction;
	void __user *addr = (void __user *)(regs->pc +
		instruction_size(old_instruction));

	if (copy_from_user(&instruction, addr, sizeof(instruction))) {
		 
		if (user_mode(regs))
			return -EFAULT;

		 
		die("delay-slot-insn faulting in handle_unaligned_delayslot",
		    regs, 0);
	}

	return handle_unaligned_ins(instruction, regs, ma);
}