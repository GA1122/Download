static int first_nibble_is_8(RAnal* anal, RAnalOp* op, ut16 code){
	if (IS_BT_OR_BF(code)) {
		op->type = R_ANAL_OP_TYPE_CJMP;  
		op->jump = disarm_8bit_offset (op->addr, GET_BTF_OFFSET(code));
		op->fail = op->addr + 2 ;
		op->eob  = true;
		if (IS_BTS(code) || IS_BFS(code))
			op->delay = 1;  
	} else if (IS_MOVB_REGDISP_R0(code)) {
		op->type = R_ANAL_OP_TYPE_LOAD;
		op->dst = anal_fill_ai_rg (anal, 0);
		op->src[0] = anal_fill_reg_disp_mem (anal, GET_SOURCE_REG(code), code&0x0F, BYTE_SIZE);
	} else if (IS_MOVW_REGDISP_R0(code)) {
		op->type = R_ANAL_OP_TYPE_LOAD;
		op->dst = anal_fill_ai_rg (anal, 0);
		op->src[0] = anal_fill_reg_disp_mem (anal, GET_SOURCE_REG(code), code&0x0F, WORD_SIZE);
	} else if (IS_CMPIMM(code)) {
		op->type = R_ANAL_OP_TYPE_CMP;
	} else if (IS_MOVB_R0_REGDISP(code)) {
		 
		op->type = R_ANAL_OP_TYPE_STORE;
		op->src[0] = anal_fill_ai_rg (anal, 0);
		op->dst = anal_fill_reg_disp_mem (anal, GET_SOURCE_REG(code), code&0x0F, BYTE_SIZE);
	} else if (IS_MOVW_R0_REGDISP(code)) {
		op->type = R_ANAL_OP_TYPE_STORE;
		op->src[0] = anal_fill_ai_rg (anal, 0);
		op->dst = anal_fill_reg_disp_mem (anal, GET_SOURCE_REG(code), code&0x0F, WORD_SIZE);
	}
	return op->size;
}
