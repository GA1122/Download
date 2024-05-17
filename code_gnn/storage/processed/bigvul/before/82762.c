INST_HANDLER (spm) {	 
	ut64 spmcsr;

	r_anal_esil_reg_read (anal->esil, "spmcsr", &spmcsr, NULL);

	ESIL_A ("0x7c,spmcsr,&=,");

	switch (spmcsr & 0x7f) {
		case 0x03:  
			ESIL_A ("16,rampz,<<,z,+,");  
			ESIL_A ("SPM_PAGE_ERASE,");   
			break;

		case 0x01:  
			ESIL_A ("r1,r0,");            
			ESIL_A ("z,");                
			ESIL_A ("SPM_PAGE_FILL,");    
			break;

		case 0x05:  
			ESIL_A ("16,rampz,<<,z,+,");  
			ESIL_A ("SPM_PAGE_WRITE,");   
			break;

		default:
			eprintf ("SPM: I dont know what to do with SPMCSR %02x.\n",
					(unsigned int) spmcsr);
	}

	op->cycles = 1;	 
}
