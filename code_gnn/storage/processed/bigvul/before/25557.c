static int handle_unaligned_ins(insn_size_t instruction, struct pt_regs *regs,
				struct mem_access *ma)
{
	int ret, index, count;
	unsigned long *rm, *rn;
	unsigned char *src, *dst;
	unsigned char __user *srcu, *dstu;

	index = (instruction>>8)&15;	 
	rn = &regs->regs[index];

	index = (instruction>>4)&15;	 
	rm = &regs->regs[index];

	count = 1<<(instruction&3);

	switch (count) {
	case 1: inc_unaligned_byte_access(); break;
	case 2: inc_unaligned_word_access(); break;
	case 4: inc_unaligned_dword_access(); break;
	case 8: inc_unaligned_multi_access(); break;
	}

	ret = -EFAULT;
	switch (instruction>>12) {
	case 0:  
		if (instruction & 8) {
			 
			srcu = (unsigned char __user *)*rm;
			srcu += regs->regs[0];
			dst = (unsigned char *)rn;
			*(unsigned long *)dst = 0;

#if !defined(__LITTLE_ENDIAN__)
			dst += 4-count;
#endif
			if (ma->from(dst, srcu, count))
				goto fetch_fault;

			sign_extend(count, dst);
		} else {
			 
			src = (unsigned char *)rm;
#if !defined(__LITTLE_ENDIAN__)
			src += 4-count;
#endif
			dstu = (unsigned char __user *)*rn;
			dstu += regs->regs[0];

			if (ma->to(dstu, src, count))
				goto fetch_fault;
		}
		ret = 0;
		break;

	case 1:  
		src = (unsigned char*) rm;
		dstu = (unsigned char __user *)*rn;
		dstu += (instruction&0x000F)<<2;

		if (ma->to(dstu, src, 4))
			goto fetch_fault;
		ret = 0;
		break;

	case 2:  
		if (instruction & 4)
			*rn -= count;
		src = (unsigned char*) rm;
		dstu = (unsigned char __user *)*rn;
#if !defined(__LITTLE_ENDIAN__)
		src += 4-count;
#endif
		if (ma->to(dstu, src, count))
			goto fetch_fault;
		ret = 0;
		break;

	case 5:  
		srcu = (unsigned char __user *)*rm;
		srcu += (instruction & 0x000F) << 2;
		dst = (unsigned char *)rn;
		*(unsigned long *)dst = 0;

		if (ma->from(dst, srcu, 4))
			goto fetch_fault;
		ret = 0;
		break;

	case 6:	 
		srcu = (unsigned char __user *)*rm;
		if (instruction & 4)
			*rm += count;
		dst = (unsigned char*) rn;
		*(unsigned long*)dst = 0;

#if !defined(__LITTLE_ENDIAN__)
		dst += 4-count;
#endif
		if (ma->from(dst, srcu, count))
			goto fetch_fault;
		sign_extend(count, dst);
		ret = 0;
		break;

	case 8:
		switch ((instruction&0xFF00)>>8) {
		case 0x81:  
			src = (unsigned char *) &regs->regs[0];
#if !defined(__LITTLE_ENDIAN__)
			src += 2;
#endif
			dstu = (unsigned char __user *)*rm;  
			dstu += (instruction & 0x000F) << 1;

			if (ma->to(dstu, src, 2))
				goto fetch_fault;
			ret = 0;
			break;

		case 0x85:  
			srcu = (unsigned char __user *)*rm;
			srcu += (instruction & 0x000F) << 1;
			dst = (unsigned char *) &regs->regs[0];
			*(unsigned long *)dst = 0;

#if !defined(__LITTLE_ENDIAN__)
			dst += 2;
#endif
			if (ma->from(dst, srcu, 2))
				goto fetch_fault;
			sign_extend(2, dst);
			ret = 0;
			break;
		}
		break;
	}
	return ret;

 fetch_fault:
	 
	die_if_no_fixup("Fault in unaligned fixup", regs, 0);
	return -EFAULT;
}
