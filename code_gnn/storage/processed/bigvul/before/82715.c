INST_HANDLER (des) {	 
	if (desctx.round < 16) {	 
		op->type = R_ANAL_OP_TYPE_CRYPTO;
		op->cycles = 1;		 
		r_strbuf_setf (&op->esil, "%d,des", desctx.round);
	}
}
