static void pdist(struct pt_regs *regs, unsigned int insn)
{
	struct fpustate *f = FPUSTATE;
	unsigned long rs1, rs2, *rd, rd_val;
	unsigned long i;

	rs1 = fpd_regval(f, RS1(insn));
	rs2 = fpd_regval(f, RS2(insn));
	rd = fpd_regaddr(f, RD(insn));

	rd_val = *rd;

	for (i = 0; i < 8; i++) {
		s16 s1, s2;

		s1 = (rs1 >> (56 - (i * 8))) & 0xff;
		s2 = (rs2 >> (56 - (i * 8))) & 0xff;

		 
		s1 -= s2;
		if (s1 < 0)
			s1 = ~s1 + 1;

		rd_val += s1;
	}

	*rd = rd_val;
}