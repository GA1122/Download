void handle_vm86_fault(struct kernel_vm86_regs *regs, long error_code)
{
	unsigned char opcode;
	unsigned char __user *csp;
	unsigned char __user *ssp;
	unsigned short ip, sp, orig_flags;
	int data32, pref_done;

#define CHECK_IF_IN_TRAP \
	if (VMPI.vm86dbg_active && VMPI.vm86dbg_TFpendig) \
		newflags |= X86_EFLAGS_TF
#define VM86_FAULT_RETURN do { \
	if (VMPI.force_return_for_pic  && (VEFLAGS & (X86_EFLAGS_IF | X86_EFLAGS_VIF))) \
		return_to_32bit(regs, VM86_PICRETURN); \
	if (orig_flags & X86_EFLAGS_TF) \
		handle_vm86_trap(regs, 0, 1); \
	return; } while (0)

	orig_flags = *(unsigned short *)&regs->pt.flags;

	csp = (unsigned char __user *) (regs->pt.cs << 4);
	ssp = (unsigned char __user *) (regs->pt.ss << 4);
	sp = SP(regs);
	ip = IP(regs);

	data32 = 0;
	pref_done = 0;
	do {
		switch (opcode = popb(csp, ip, simulate_sigsegv)) {
		case 0x66:            data32 = 1; break;
		case 0x67:         break;
		case 0x2e:                     break;
		case 0x3e:                     break;
		case 0x26:                     break;
		case 0x36:                     break;
		case 0x65:                     break;
		case 0x64:                     break;
		case 0xf2:              break;
		case 0xf3:                    break;
		default: pref_done = 1;
		}
	} while (!pref_done);

	switch (opcode) {

	 
	case 0x9c:
		if (data32) {
			pushl(ssp, sp, get_vflags(regs), simulate_sigsegv);
			SP(regs) -= 4;
		} else {
			pushw(ssp, sp, get_vflags(regs), simulate_sigsegv);
			SP(regs) -= 2;
		}
		IP(regs) = ip;
		VM86_FAULT_RETURN;

	 
	case 0x9d:
		{
		unsigned long newflags;
		if (data32) {
			newflags = popl(ssp, sp, simulate_sigsegv);
			SP(regs) += 4;
		} else {
			newflags = popw(ssp, sp, simulate_sigsegv);
			SP(regs) += 2;
		}
		IP(regs) = ip;
		CHECK_IF_IN_TRAP;
		if (data32)
			set_vflags_long(newflags, regs);
		else
			set_vflags_short(newflags, regs);

		VM86_FAULT_RETURN;
		}

	 
	case 0xcd: {
		int intno = popb(csp, ip, simulate_sigsegv);
		IP(regs) = ip;
		if (VMPI.vm86dbg_active) {
			if ((1 << (intno & 7)) & VMPI.vm86dbg_intxxtab[intno >> 3])
				return_to_32bit(regs, VM86_INTx + (intno << 8));
		}
		do_int(regs, intno, ssp, sp);
		return;
	}

	 
	case 0xcf:
		{
		unsigned long newip;
		unsigned long newcs;
		unsigned long newflags;
		if (data32) {
			newip = popl(ssp, sp, simulate_sigsegv);
			newcs = popl(ssp, sp, simulate_sigsegv);
			newflags = popl(ssp, sp, simulate_sigsegv);
			SP(regs) += 12;
		} else {
			newip = popw(ssp, sp, simulate_sigsegv);
			newcs = popw(ssp, sp, simulate_sigsegv);
			newflags = popw(ssp, sp, simulate_sigsegv);
			SP(regs) += 6;
		}
		IP(regs) = newip;
		regs->pt.cs = newcs;
		CHECK_IF_IN_TRAP;
		if (data32) {
			set_vflags_long(newflags, regs);
		} else {
			set_vflags_short(newflags, regs);
		}
		VM86_FAULT_RETURN;
		}

	 
	case 0xfa:
		IP(regs) = ip;
		clear_IF(regs);
		VM86_FAULT_RETURN;

	 
	 
	case 0xfb:
		IP(regs) = ip;
		set_IF(regs);
		VM86_FAULT_RETURN;

	default:
		return_to_32bit(regs, VM86_UNKNOWN);
	}

	return;

simulate_sigsegv:
	 
	return_to_32bit(regs, VM86_UNKNOWN);
}