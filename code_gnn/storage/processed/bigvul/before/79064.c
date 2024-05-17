unsigned paravirt_patch_default(u8 type, u16 clobbers, void *insnbuf,
				unsigned long addr, unsigned len)
{
	void *opfunc = get_call_destination(type);
	unsigned ret;

	if (opfunc == NULL)
		 
		ret = paravirt_patch_insns(insnbuf, len, ud2a, ud2a+sizeof(ud2a));
	else if (opfunc == _paravirt_nop)
		ret = 0;

	 
	else if (opfunc == _paravirt_ident_32)
		ret = paravirt_patch_ident_32(insnbuf, len);
	else if (opfunc == _paravirt_ident_64)
		ret = paravirt_patch_ident_64(insnbuf, len);

	else if (type == PARAVIRT_PATCH(pv_cpu_ops.iret) ||
		 type == PARAVIRT_PATCH(pv_cpu_ops.usergs_sysret64))
		 
		ret = paravirt_patch_jmp(insnbuf, opfunc, addr, len);
	else
		 
		ret = paravirt_patch_call(insnbuf, opfunc, CLBR_ANY,
					  addr, clobbers, len);

	return ret;
}
