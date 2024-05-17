INST_HANDLER (reti) {	 
	op->family = R_ANAL_OP_FAMILY_PRIV;

	INST_CALL (ret);

	ESIL_A ("1,if,=,");
}
