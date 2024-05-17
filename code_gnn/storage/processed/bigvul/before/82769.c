static void __generic_bitop_flags(RAnalOp *op) {
	ESIL_A ("0,vf,=,");					 
	ESIL_A ("0,RPICK,0x80,&,!,!,nf,=,");			 
	ESIL_A ("0,RPICK,!,zf,=,");				 
	ESIL_A ("vf,nf,^,sf,=,");				 
}
