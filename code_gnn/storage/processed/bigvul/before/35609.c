static int writeback(struct x86_emulate_ctxt *ctxt, struct operand *op)
{
	switch (op->type) {
	case OP_REG:
		write_register_operand(op);
		break;
	case OP_MEM:
		if (ctxt->lock_prefix)
			return segmented_cmpxchg(ctxt,
						 op->addr.mem,
						 &op->orig_val,
						 &op->val,
						 op->bytes);
		else
			return segmented_write(ctxt,
					       op->addr.mem,
					       &op->val,
					       op->bytes);
		break;
	case OP_MEM_STR:
		return segmented_write(ctxt,
				       op->addr.mem,
				       op->data,
				       op->bytes * op->count);
		break;
	case OP_XMM:
		write_sse_reg(ctxt, &op->vec_val, op->addr.xmm);
		break;
	case OP_MM:
		write_mmx_reg(ctxt, &op->mm_val, op->addr.mm);
		break;
	case OP_NONE:
		 
		break;
	default:
		break;
	}
	return X86EMUL_CONTINUE;
}