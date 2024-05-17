static int misaligned_fixup(struct pt_regs *regs)
{
	unsigned long opcode;
	int error;
	int major, minor;

	if (!user_mode_unaligned_fixup_enable)
		return -1;

	error = read_opcode(regs->pc, &opcode, user_mode(regs));
	if (error < 0) {
		return error;
	}
	major = (opcode >> 26) & 0x3f;
	minor = (opcode >> 16) & 0xf;

	if (user_mode(regs) && (user_mode_unaligned_fixup_count > 0)) {
		--user_mode_unaligned_fixup_count;
		 
		printk("Fixing up unaligned userspace access in \"%s\" pid=%d pc=0x%08x ins=0x%08lx\n",
		       current->comm, task_pid_nr(current), (__u32)regs->pc, opcode);
	} else if (!user_mode(regs) && (kernel_mode_unaligned_fixup_count > 0)) {
		--kernel_mode_unaligned_fixup_count;
		if (in_interrupt()) {
			printk("Fixing up unaligned kernelspace access in interrupt pc=0x%08x ins=0x%08lx\n",
			       (__u32)regs->pc, opcode);
		} else {
			printk("Fixing up unaligned kernelspace access in \"%s\" pid=%d pc=0x%08x ins=0x%08lx\n",
			       current->comm, task_pid_nr(current), (__u32)regs->pc, opcode);
		}
	}


	switch (major) {
		case (0x84>>2):  
			error = misaligned_load(regs, opcode, 1, 1, 1);
			break;
		case (0xb0>>2):  
			error = misaligned_load(regs, opcode, 1, 1, 0);
			break;
		case (0x88>>2):  
			error = misaligned_load(regs, opcode, 1, 2, 1);
			break;
		case (0x8c>>2):  
			error = misaligned_load(regs, opcode, 1, 3, 0);
			break;

		case (0xa4>>2):  
			error = misaligned_store(regs, opcode, 1, 1);
			break;
		case (0xa8>>2):  
			error = misaligned_store(regs, opcode, 1, 2);
			break;
		case (0xac>>2):  
			error = misaligned_store(regs, opcode, 1, 3);
			break;

		case (0x40>>2):  
			switch (minor) {
				case 0x1:  
					error = misaligned_load(regs, opcode, 0, 1, 1);
					break;
				case 0x5:  
					error = misaligned_load(regs, opcode, 0, 1, 0);
					break;
				case 0x2:  
					error = misaligned_load(regs, opcode, 0, 2, 1);
					break;
				case 0x3:  
					error = misaligned_load(regs, opcode, 0, 3, 0);
					break;
				default:
					error = -1;
					break;
			}
			break;

		case (0x60>>2):  
			switch (minor) {
				case 0x1:  
					error = misaligned_store(regs, opcode, 0, 1);
					break;
				case 0x2:  
					error = misaligned_store(regs, opcode, 0, 2);
					break;
				case 0x3:  
					error = misaligned_store(regs, opcode, 0, 3);
					break;
				default:
					error = -1;
					break;
			}
			break;

		case (0x94>>2):  
			error = misaligned_fpu_load(regs, opcode, 1, 2, 0);
			break;
		case (0x98>>2):  
			error = misaligned_fpu_load(regs, opcode, 1, 3, 1);
			break;
		case (0x9c>>2):  
			error = misaligned_fpu_load(regs, opcode, 1, 3, 0);
			break;
		case (0x1c>>2):  
			switch (minor) {
			case 0x8:  
				error = misaligned_fpu_load(regs, opcode, 0, 2, 0);
				break;
			case 0xd:  
				error = misaligned_fpu_load(regs, opcode, 0, 3, 1);
				break;
			case 0x9:  
				error = misaligned_fpu_load(regs, opcode, 0, 3, 0);
				break;
			default:
				error = -1;
				break;
			}
			break;
		case (0xb4>>2):  
			error = misaligned_fpu_store(regs, opcode, 1, 2, 0);
			break;
		case (0xb8>>2):  
			error = misaligned_fpu_store(regs, opcode, 1, 3, 1);
			break;
		case (0xbc>>2):  
			error = misaligned_fpu_store(regs, opcode, 1, 3, 0);
			break;
		case (0x3c>>2):  
			switch (minor) {
			case 0x8:  
				error = misaligned_fpu_store(regs, opcode, 0, 2, 0);
				break;
			case 0xd:  
				error = misaligned_fpu_store(regs, opcode, 0, 3, 1);
				break;
			case 0x9:  
				error = misaligned_fpu_store(regs, opcode, 0, 3, 0);
				break;
			default:
				error = -1;
				break;
			}
			break;

		default:
			 
			error = -1;
			break;
	}

	if (error < 0) {
		return error;
	} else {
		regs->pc += 4;  
		return 0;
	}

}
