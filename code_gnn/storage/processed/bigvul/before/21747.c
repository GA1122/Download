static int em_grp2(struct x86_emulate_ctxt *ctxt)
{
	switch (ctxt->modrm_reg) {
	case 0:	 
		emulate_2op_SrcB(ctxt, "rol");
		break;
	case 1:	 
		emulate_2op_SrcB(ctxt, "ror");
		break;
	case 2:	 
		emulate_2op_SrcB(ctxt, "rcl");
		break;
	case 3:	 
		emulate_2op_SrcB(ctxt, "rcr");
		break;
	case 4:	 
	case 6:	 
		emulate_2op_SrcB(ctxt, "sal");
		break;
	case 5:	 
		emulate_2op_SrcB(ctxt, "shr");
		break;
	case 7:	 
		emulate_2op_SrcB(ctxt, "sar");
		break;
	}
	return X86EMUL_CONTINUE;
}
