static int get_seg_reg_override_idx(struct insn *insn)
{
	int idx = INAT_SEG_REG_DEFAULT;
	int num_overrides = 0, i;

	insn_get_prefixes(insn);

	 
	for (i = 0; i < insn->prefixes.nbytes; i++) {
		insn_attr_t attr;

		attr = inat_get_opcode_attribute(insn->prefixes.bytes[i]);
		switch (attr) {
		case INAT_MAKE_PREFIX(INAT_PFX_CS):
			idx = INAT_SEG_REG_CS;
			num_overrides++;
			break;
		case INAT_MAKE_PREFIX(INAT_PFX_SS):
			idx = INAT_SEG_REG_SS;
			num_overrides++;
			break;
		case INAT_MAKE_PREFIX(INAT_PFX_DS):
			idx = INAT_SEG_REG_DS;
			num_overrides++;
			break;
		case INAT_MAKE_PREFIX(INAT_PFX_ES):
			idx = INAT_SEG_REG_ES;
			num_overrides++;
			break;
		case INAT_MAKE_PREFIX(INAT_PFX_FS):
			idx = INAT_SEG_REG_FS;
			num_overrides++;
			break;
		case INAT_MAKE_PREFIX(INAT_PFX_GS):
			idx = INAT_SEG_REG_GS;
			num_overrides++;
			break;
		 
		}
	}

	 
	if (num_overrides > 1)
		return -EINVAL;

	return idx;
}