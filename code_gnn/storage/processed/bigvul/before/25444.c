static int fpux_emu(struct pt_regs *xcp, struct mips_fpu_struct *ctx,
	mips_instruction ir, void *__user *fault_addr)
{
	unsigned rcsr = 0;	 

	MIPS_FPU_EMU_INC_STATS(cp1xops);

	switch (MIPSInst_FMA_FFMT(ir)) {
	case s_fmt:{		 

		ieee754sp(*handler) (ieee754sp, ieee754sp, ieee754sp);
		ieee754sp fd, fr, fs, ft;
		u32 __user *va;
		u32 val;

		switch (MIPSInst_FUNC(ir)) {
		case lwxc1_op:
			va = (void __user *) (xcp->regs[MIPSInst_FR(ir)] +
				xcp->regs[MIPSInst_FT(ir)]);

			MIPS_FPU_EMU_INC_STATS(loads);
			if (!access_ok(VERIFY_READ, va, sizeof(u32))) {
				MIPS_FPU_EMU_INC_STATS(errors);
				*fault_addr = va;
				return SIGBUS;
			}
			if (__get_user(val, va)) {
				MIPS_FPU_EMU_INC_STATS(errors);
				*fault_addr = va;
				return SIGSEGV;
			}
			SITOREG(val, MIPSInst_FD(ir));
			break;

		case swxc1_op:
			va = (void __user *) (xcp->regs[MIPSInst_FR(ir)] +
				xcp->regs[MIPSInst_FT(ir)]);

			MIPS_FPU_EMU_INC_STATS(stores);

			SIFROMREG(val, MIPSInst_FS(ir));
			if (!access_ok(VERIFY_WRITE, va, sizeof(u32))) {
				MIPS_FPU_EMU_INC_STATS(errors);
				*fault_addr = va;
				return SIGBUS;
			}
			if (put_user(val, va)) {
				MIPS_FPU_EMU_INC_STATS(errors);
				*fault_addr = va;
				return SIGSEGV;
			}
			break;

		case madd_s_op:
			handler = fpemu_sp_madd;
			goto scoptop;
		case msub_s_op:
			handler = fpemu_sp_msub;
			goto scoptop;
		case nmadd_s_op:
			handler = fpemu_sp_nmadd;
			goto scoptop;
		case nmsub_s_op:
			handler = fpemu_sp_nmsub;
			goto scoptop;

		      scoptop:
			SPFROMREG(fr, MIPSInst_FR(ir));
			SPFROMREG(fs, MIPSInst_FS(ir));
			SPFROMREG(ft, MIPSInst_FT(ir));
			fd = (*handler) (fr, fs, ft);
			SPTOREG(fd, MIPSInst_FD(ir));

		      copcsr:
			if (ieee754_cxtest(IEEE754_INEXACT))
				rcsr |= FPU_CSR_INE_X | FPU_CSR_INE_S;
			if (ieee754_cxtest(IEEE754_UNDERFLOW))
				rcsr |= FPU_CSR_UDF_X | FPU_CSR_UDF_S;
			if (ieee754_cxtest(IEEE754_OVERFLOW))
				rcsr |= FPU_CSR_OVF_X | FPU_CSR_OVF_S;
			if (ieee754_cxtest(IEEE754_INVALID_OPERATION))
				rcsr |= FPU_CSR_INV_X | FPU_CSR_INV_S;

			ctx->fcr31 = (ctx->fcr31 & ~FPU_CSR_ALL_X) | rcsr;
			if ((ctx->fcr31 >> 5) & ctx->fcr31 & FPU_CSR_ALL_E) {
				 
				return SIGFPE;
			}

			break;

		default:
			return SIGILL;
		}
		break;
	}

	case d_fmt:{		 
		ieee754dp(*handler) (ieee754dp, ieee754dp, ieee754dp);
		ieee754dp fd, fr, fs, ft;
		u64 __user *va;
		u64 val;

		switch (MIPSInst_FUNC(ir)) {
		case ldxc1_op:
			va = (void __user *) (xcp->regs[MIPSInst_FR(ir)] +
				xcp->regs[MIPSInst_FT(ir)]);

			MIPS_FPU_EMU_INC_STATS(loads);
			if (!access_ok(VERIFY_READ, va, sizeof(u64))) {
				MIPS_FPU_EMU_INC_STATS(errors);
				*fault_addr = va;
				return SIGBUS;
			}
			if (__get_user(val, va)) {
				MIPS_FPU_EMU_INC_STATS(errors);
				*fault_addr = va;
				return SIGSEGV;
			}
			DITOREG(val, MIPSInst_FD(ir));
			break;

		case sdxc1_op:
			va = (void __user *) (xcp->regs[MIPSInst_FR(ir)] +
				xcp->regs[MIPSInst_FT(ir)]);

			MIPS_FPU_EMU_INC_STATS(stores);
			DIFROMREG(val, MIPSInst_FS(ir));
			if (!access_ok(VERIFY_WRITE, va, sizeof(u64))) {
				MIPS_FPU_EMU_INC_STATS(errors);
				*fault_addr = va;
				return SIGBUS;
			}
			if (__put_user(val, va)) {
				MIPS_FPU_EMU_INC_STATS(errors);
				*fault_addr = va;
				return SIGSEGV;
			}
			break;

		case madd_d_op:
			handler = fpemu_dp_madd;
			goto dcoptop;
		case msub_d_op:
			handler = fpemu_dp_msub;
			goto dcoptop;
		case nmadd_d_op:
			handler = fpemu_dp_nmadd;
			goto dcoptop;
		case nmsub_d_op:
			handler = fpemu_dp_nmsub;
			goto dcoptop;

		      dcoptop:
			DPFROMREG(fr, MIPSInst_FR(ir));
			DPFROMREG(fs, MIPSInst_FS(ir));
			DPFROMREG(ft, MIPSInst_FT(ir));
			fd = (*handler) (fr, fs, ft);
			DPTOREG(fd, MIPSInst_FD(ir));
			goto copcsr;

		default:
			return SIGILL;
		}
		break;
	}

	case 0x7:		 
		if (MIPSInst_FUNC(ir) != pfetch_op) {
			return SIGILL;
		}
		 
		break;

	default:
		return SIGILL;
	}

	return 0;
}