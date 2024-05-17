 static int load_segment_descriptor(struct x86_emulate_ctxt *ctxt,
  				   u16 selector, int seg)
  {
  	u8 cpl = ctxt->ops->cpl(ctxt);
// 
// 	 
// 	if (seg == VCPU_SREG_SS && selector == 3 &&
// 	    ctxt->mode == X86EMUL_MODE_PROT64)
// 		return emulate_exception(ctxt, GP_VECTOR, 0, true);
// 
  	return __load_segment_descriptor(ctxt, selector, seg, cpl,
  					 X86_TRANSFER_NONE, NULL);
  }