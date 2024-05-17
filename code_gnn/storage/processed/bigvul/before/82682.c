static int first_nibble_is_0(RAnal* anal, RAnalOp* op, ut16 code){
	if(IS_BSRF(code)) {
		 
		op->type = R_ANAL_OP_TYPE_UCALL;
		op->delay = 1;
		op->dst = anal_regrel_jump (anal, op, GET_TARGET_REG(code));
	} else if (IS_BRAF(code)) {
		 
		op->type = R_ANAL_OP_TYPE_UJMP;
		op->dst = anal_regrel_jump (anal, op, GET_TARGET_REG(code));
		op->eob = true;
	} else if( IS_RTS(code) ) {
		 
		op->type = R_ANAL_OP_TYPE_RET;
		op->delay = 1;
		op->eob = true;
	} else if (IS_RTE(code)) {
		op->type = R_ANAL_OP_TYPE_RET;
		op->delay = 1;
		op->eob = true;
	} else if (IS_MOVB_REG_TO_R0REL(code)) {	 
		op->type = R_ANAL_OP_TYPE_STORE;
		op->src[0] = anal_fill_ai_rg (anal, GET_SOURCE_REG(code));
		op->dst = anal_fill_r0_reg_ref (anal, GET_TARGET_REG(code), BYTE_SIZE);
	} else if (IS_MOVW_REG_TO_R0REL(code)) {
		op->type = R_ANAL_OP_TYPE_STORE;
		op->src[0] = anal_fill_ai_rg (anal, GET_SOURCE_REG(code));
		op->dst = anal_fill_r0_reg_ref (anal, GET_TARGET_REG(code), WORD_SIZE);
	} else if (IS_MOVL_REG_TO_R0REL(code)) {
		op->type = R_ANAL_OP_TYPE_STORE;
		op->src[0] = anal_fill_ai_rg (anal, GET_SOURCE_REG(code));
		op->dst = anal_fill_r0_reg_ref(anal, GET_TARGET_REG(code), LONG_SIZE);
	} else if (IS_MOVB_R0REL_TO_REG(code)) {
		op->type = R_ANAL_OP_TYPE_LOAD;
		op->src[0] = anal_fill_r0_reg_ref (anal, GET_SOURCE_REG(code), BYTE_SIZE);
		op->dst = anal_fill_ai_rg (anal, GET_TARGET_REG(code));
	} else if (IS_MOVW_R0REL_TO_REG(code)) {
		op->type = R_ANAL_OP_TYPE_LOAD;
		op->src[0] = anal_fill_r0_reg_ref (anal, GET_SOURCE_REG(code), WORD_SIZE);
		op->dst = anal_fill_ai_rg (anal, GET_TARGET_REG(code));
	} else if (IS_MOVL_R0REL_TO_REG(code)) {
		op->type = R_ANAL_OP_TYPE_LOAD;
		op->src[0] = anal_fill_r0_reg_ref (anal, GET_SOURCE_REG(code), LONG_SIZE);
		op->dst = anal_fill_ai_rg (anal, GET_TARGET_REG(code));
	} else if (IS_NOP(code)) {
		op->type = R_ANAL_OP_TYPE_NOP;
	} else if (IS_CLRT(code)) {
		op->type = R_ANAL_OP_TYPE_UNK;	 
	} else if (IS_SETT(code)) {
		op->type = R_ANAL_OP_TYPE_UNK;
	} else if (IS_CLRMAC(code)) {
		op->type = R_ANAL_OP_TYPE_UNK;	 
	} else if (IS_DIV0U(code)) {
		op->type = R_ANAL_OP_TYPE_DIV;
	} else if (IS_MOVT(code)) {
		op->type = R_ANAL_OP_TYPE_MOV;
		op->dst = anal_fill_ai_rg (anal, GET_TARGET_REG(code));
	} else if (IS_MULL(code)) {
		op->type = R_ANAL_OP_TYPE_MUL;
		op->src[0] = anal_fill_ai_rg (anal, GET_TARGET_REG(code));
		op->src[1] = anal_fill_ai_rg (anal, GET_SOURCE_REG(code));
	} else if (IS_SLEEP(code)) {
		op->type = R_ANAL_OP_TYPE_UNK;
	} else if (IS_STSMAC(code)) {	 
		op->type = R_ANAL_OP_TYPE_MOV;
		op->dst = anal_fill_ai_rg (anal, GET_TARGET_REG(code));
	} else if (IS_STCSR1(code)) {	 
		op->type = R_ANAL_OP_TYPE_MOV;
		op->dst = anal_fill_ai_rg (anal, GET_TARGET_REG(code));
	} else if (IS_STSPR(code)) {	 
		op->type = R_ANAL_OP_TYPE_MOV;
		op->dst = anal_fill_ai_rg (anal, GET_TARGET_REG(code));
	}

	return op->size;
}