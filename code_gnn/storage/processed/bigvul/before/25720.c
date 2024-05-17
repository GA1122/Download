static void pmul(struct pt_regs *regs, unsigned int insn, unsigned int opf)
{
	struct fpustate *f = FPUSTATE;
	unsigned long rs1, rs2, rd_val;

	switch (opf) {
	case FMUL8x16_OPF: {
		unsigned long byte;

		rs1 = fps_regval(f, RS1(insn));
		rs2 = fpd_regval(f, RS2(insn));

		rd_val = 0;
		for (byte = 0; byte < 4; byte++) {
			u16 src1 = (rs1 >> (byte *  8)) & 0x00ff;
			s16 src2 = (rs2 >> (byte * 16)) & 0xffff;
			u32 prod = src1 * src2;
			u16 scaled = ((prod & 0x00ffff00) >> 8);

			 
			if (prod & 0x80)
				scaled++;
			rd_val |= ((scaled & 0xffffUL) << (byte * 16UL));
		}

		*fpd_regaddr(f, RD(insn)) = rd_val;
		break;
	}

	case FMUL8x16AU_OPF:
	case FMUL8x16AL_OPF: {
		unsigned long byte;
		s16 src2;

		rs1 = fps_regval(f, RS1(insn));
		rs2 = fps_regval(f, RS2(insn));

		rd_val = 0;
		src2 = rs2 >> (opf == FMUL8x16AU_OPF ? 16 : 0);
		for (byte = 0; byte < 4; byte++) {
			u16 src1 = (rs1 >> (byte * 8)) & 0x00ff;
			u32 prod = src1 * src2;
			u16 scaled = ((prod & 0x00ffff00) >> 8);

			 
			if (prod & 0x80)
				scaled++;
			rd_val |= ((scaled & 0xffffUL) << (byte * 16UL));
		}

		*fpd_regaddr(f, RD(insn)) = rd_val;
		break;
	}

	case FMUL8SUx16_OPF:
	case FMUL8ULx16_OPF: {
		unsigned long byte, ushift;

		rs1 = fpd_regval(f, RS1(insn));
		rs2 = fpd_regval(f, RS2(insn));

		rd_val = 0;
		ushift = (opf == FMUL8SUx16_OPF) ? 8 : 0;
		for (byte = 0; byte < 4; byte++) {
			u16 src1;
			s16 src2;
			u32 prod;
			u16 scaled;

			src1 = ((rs1 >> ((16 * byte) + ushift)) & 0x00ff);
			src2 = ((rs2 >> (16 * byte)) & 0xffff);
			prod = src1 * src2;
			scaled = ((prod & 0x00ffff00) >> 8);

			 
			if (prod & 0x80)
				scaled++;
			rd_val |= ((scaled & 0xffffUL) << (byte * 16UL));
		}

		*fpd_regaddr(f, RD(insn)) = rd_val;
		break;
	}

	case FMULD8SUx16_OPF:
	case FMULD8ULx16_OPF: {
		unsigned long byte, ushift;

		rs1 = fps_regval(f, RS1(insn));
		rs2 = fps_regval(f, RS2(insn));

		rd_val = 0;
		ushift = (opf == FMULD8SUx16_OPF) ? 8 : 0;
		for (byte = 0; byte < 2; byte++) {
			u16 src1;
			s16 src2;
			u32 prod;
			u16 scaled;

			src1 = ((rs1 >> ((16 * byte) + ushift)) & 0x00ff);
			src2 = ((rs2 >> (16 * byte)) & 0xffff);
			prod = src1 * src2;
			scaled = ((prod & 0x00ffff00) >> 8);

			 
			if (prod & 0x80)
				scaled++;
			rd_val |= ((scaled & 0xffffUL) <<
				   ((byte * 32UL) + 7UL));
		}
		*fpd_regaddr(f, RD(insn)) = rd_val;
		break;
	}
	}
}