static void __generic_push(RAnalOp *op, int sz) {
	ESIL_A ("sp,_ram,+,");			 
	if (sz > 1) {
		ESIL_A ("-%d,+,", sz - 1);	 
	}
	ESIL_A ("=[%d],", sz);			 
	ESIL_A ("-%d,sp,+=,", sz);		 
}
