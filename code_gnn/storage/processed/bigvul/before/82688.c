static int first_nibble_is_c(RAnal* anal, RAnalOp* op, ut16 code){
	if (IS_TRAP(code)) {
		op->type = R_ANAL_OP_TYPE_SWI;
		op->val = (ut8)(code&0xFF);
	} else if (IS_MOVA_PCREL_R0(code)) {
		op->type = R_ANAL_OP_TYPE_LEA;
		op->src[0] = anal_pcrel_disp_mov (anal, op, code&0xFF, LONG_SIZE);	 
		op->dst = anal_fill_ai_rg (anal, 0);  
	} else if (IS_BINLOGIC_IMM_R0(code)) {	 
		op->src[0] = anal_fill_im (anal, code&0xFF);
		op->src[1] = anal_fill_ai_rg (anal, 0);	 
		op->dst = anal_fill_ai_rg (anal, 0);  
		switch (code & 0xFF00) {
		case 0xC800:	 
			op->type = R_ANAL_OP_TYPE_ACMP;
			break;
		case 0xC900:	 
			op->type = R_ANAL_OP_TYPE_AND;
			break;
		case 0xCA00:	 
			op->type = R_ANAL_OP_TYPE_XOR;
			break;
		case 0xCB00:	 
			op->type = R_ANAL_OP_TYPE_OR;
			break;
		}
	} else if (IS_BINLOGIC_IMM_GBR(code)) {	 
		op->src[0] = anal_fill_im (anal, code&0xFF);
		switch (code & 0xFF00) {
		case 0xCC00:	 
			op->type = R_ANAL_OP_TYPE_ACMP;
			break;
		case 0xCD00:	 
			op->type = R_ANAL_OP_TYPE_AND;
			break;
		case 0xCE00:	 
			op->type = R_ANAL_OP_TYPE_XOR;
			break;
		case 0xCF00:	 
			op->type = R_ANAL_OP_TYPE_OR;
			break;
		}
	} else if (IS_MOVB_R0_GBRREF(code)) {	 
		op->type = R_ANAL_OP_TYPE_STORE;
		op->src[0] = anal_fill_ai_rg (anal, 0);
	} else if (IS_MOVW_R0_GBRREF(code)) {	 
		op->type = R_ANAL_OP_TYPE_STORE;
		op->src[0] = anal_fill_ai_rg (anal, 0);
	} else if (IS_MOVL_R0_GBRREF(code)) {	 
		op->type = R_ANAL_OP_TYPE_STORE;
		op->src[0] = anal_fill_ai_rg (anal, 0);
	} else if (IS_MOVB_GBRREF_R0(code)) {	 
		op->type = R_ANAL_OP_TYPE_LOAD;
		op->dst = anal_fill_ai_rg (anal, 0);
	} else if (IS_MOVW_GBRREF_R0(code)) {	 
		op->type = R_ANAL_OP_TYPE_LOAD;
		op->dst = anal_fill_ai_rg (anal, 0);
	} else if (IS_MOVL_GBRREF_R0(code)) {	 
		op->type = R_ANAL_OP_TYPE_LOAD;
		op->dst = anal_fill_ai_rg (anal, 0);
	}

	return op->size;
}
