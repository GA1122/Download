INST_HANDLER (sbix) {	 
	int a = (buf[0] >> 3) & 0x1f;
	int b = buf[0] & 0x07;
	RAnalOp next_op;
	RStrBuf *io_port;

	op->type2 = 0;
	op->val = a;
	op->family = R_ANAL_OP_FAMILY_IO;

	avr_op_analyze (anal,
			&next_op,
			op->addr + op->size, buf + op->size,
			len - op->size,
			cpu);
	r_strbuf_fini (&next_op.esil);
	op->jump = op->addr + next_op.size + 2;

	op->cycles = 1;	 

	io_port = __generic_io_dest (a, 0, cpu);
	ESIL_A ("%d,1,<<,%s,&,", b, io_port);		 
	ESIL_A ((buf[1] & 0xe) == 0xc
			? "!,"				 
			: "!,!,");			 
	ESIL_A ("?{,%"PFMT64d",pc,=,},", op->jump);	 
	r_strbuf_free (io_port);
}
