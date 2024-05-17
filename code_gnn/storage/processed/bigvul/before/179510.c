 static int em_call_far(struct x86_emulate_ctxt *ctxt)
 {
  	u16 sel, old_cs;
  	ulong old_eip;
  	int rc;
// 	struct desc_struct old_desc, new_desc;
// 	const struct x86_emulate_ops *ops = ctxt->ops;
// 	int cpl = ctxt->ops->cpl(ctxt);
  
	old_cs = get_segment_selector(ctxt, VCPU_SREG_CS);
  	old_eip = ctxt->_eip;
// 	ops->get_segment(ctxt, &old_cs, &old_desc, NULL, VCPU_SREG_CS);
  
  	memcpy(&sel, ctxt->src.valptr + ctxt->op_bytes, 2);
	if (load_segment_descriptor(ctxt, sel, VCPU_SREG_CS))
// 	rc = __load_segment_descriptor(ctxt, sel, VCPU_SREG_CS, cpl, false,
// 				       &new_desc);
// 	if (rc != X86EMUL_CONTINUE)
  		return X86EMUL_CONTINUE;
  
	ctxt->_eip = 0;
	memcpy(&ctxt->_eip, ctxt->src.valptr, ctxt->op_bytes);
// 	rc = assign_eip_far(ctxt, ctxt->src.val, new_desc.l);
// 	if (rc != X86EMUL_CONTINUE)
// 		goto fail;
  
  	ctxt->src.val = old_cs;
  	rc = em_push(ctxt);
  	if (rc != X86EMUL_CONTINUE)
		return rc;
// 		goto fail;
  
  	ctxt->src.val = old_eip;
	return em_push(ctxt);
// 	rc = em_push(ctxt);
// 	 
// 	if (rc != X86EMUL_CONTINUE)
// 		goto fail;
// 	return rc;
// fail:
// 	ops->set_segment(ctxt, old_cs, &old_desc, 0, VCPU_SREG_CS);
// 	return rc;
// 
  }