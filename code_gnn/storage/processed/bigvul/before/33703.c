unsigned long convert_ip_to_linear(struct task_struct *child, struct pt_regs *regs)
{
	unsigned long addr, seg;

	addr = regs->ip;
	seg = regs->cs & 0xffff;
	if (v8086_mode(regs)) {
		addr = (addr & 0xffff) + (seg << 4);
		return addr;
	}

	 
	if ((seg & SEGMENT_TI_MASK) == SEGMENT_LDT) {
		struct desc_struct *desc;
		unsigned long base;

		seg &= ~7UL;

		mutex_lock(&child->mm->context.lock);
		if (unlikely((seg >> 3) >= child->mm->context.size))
			addr = -1L;  
		else {
			desc = child->mm->context.ldt + seg;
			base = get_desc_base(desc);

			 
			if (!desc->d)
				addr &= 0xffff;
			addr += base;
		}
		mutex_unlock(&child->mm->context.lock);
	}

	return addr;
}