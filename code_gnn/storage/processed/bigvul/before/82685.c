static int first_nibble_is_4(RAnal* anal, RAnalOp* op, ut16 code){
	switch (code & 0xF0FF) {
	case 0x4020:	 
		op->type = R_ANAL_OP_TYPE_SAL;
		break;
	case 0x4021:	 
		op->type = R_ANAL_OP_TYPE_SAR;
		break;
	case 0x4000:	 
	case 0x4008:	 
	case 0x4018:	 
	case 0x4028:	 
		op->type = R_ANAL_OP_TYPE_SHL;
		break;
	case 0x4001:	 
	case 0x4009:	 
	case 0x4019:	 
	case 0x4029:	 
		op->type = R_ANAL_OP_TYPE_SHR;
		break;
	default:
		break;
	}

	if (IS_JSR(code)) {
		op->type = R_ANAL_OP_TYPE_UCALL;  
		op->delay = 1;
		op->dst = anal_fill_ai_rg (anal, GET_TARGET_REG(code));
	} else if ( IS_JMP(code) ) {
		op->type = R_ANAL_OP_TYPE_UJMP;  
		op->dst = anal_fill_ai_rg (anal, GET_TARGET_REG(code));
		op->delay = 1;
		op->eob = true;
	} else if (IS_CMPPL(code) || IS_CMPPZ(code)) {
		op->type = R_ANAL_OP_TYPE_CMP;
	} else if (IS_LDCLSR1(code) || IS_LDSLMAC(code) || IS_LDSLPR(code)) {
		op->type = R_ANAL_OP_TYPE_POP;
	} else if (IS_LDCSR1(code) || IS_LDSMAC(code) || IS_LDSPR(code)) {
		op->type = R_ANAL_OP_TYPE_MOV;
	} else if (IS_ROT(code)) {
		op->type = (code&1)? R_ANAL_OP_TYPE_ROR:R_ANAL_OP_TYPE_ROL;
	} else if (IS_STCLSR1(code) || IS_STSLMAC(code) || IS_STSLPR(code)) {
		op->type = R_ANAL_OP_TYPE_PUSH;
	} else if (IS_TASB(code)) {
		op->type = R_ANAL_OP_TYPE_UNK;
	} else if (IS_DT(code)) {
		op->type = R_ANAL_OP_TYPE_UNK;
	}
	return op->size;
}
