 static inline void jmp_rel(struct x86_emulate_ctxt *ctxt, int rel)
// static inline int jmp_rel(struct x86_emulate_ctxt *ctxt, int rel)
  {
	assign_eip_near(ctxt, ctxt->_eip + rel);
// 	return assign_eip_near(ctxt, ctxt->_eip + rel);
  }