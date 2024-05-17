static void __generic_pop(RAnalOp *op, int sz) {
	if (sz > 1) {
		ESIL_A ("1,sp,+,_ram,+,");	 
		ESIL_A ("[%d],", sz);		 
		ESIL_A ("%d,sp,+=,", sz);	 
	} else {
		ESIL_A ("1,sp,+=,"		 
			"sp,_ram,+,[1],");	 
	}
}
