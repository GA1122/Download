ia64_patch_imm60 (u64 insn_addr, u64 val)
{
	 
  	insn_addr &= -16UL;
	ia64_patch(insn_addr + 2,
		   0x011ffffe000UL, (  ((val & 0x0800000000000000UL) >> 23)  
				     | ((val & 0x00000000000fffffUL) << 13)  ));
	ia64_patch(insn_addr + 1, 0x1fffffffffcUL, val >> 18);
}
