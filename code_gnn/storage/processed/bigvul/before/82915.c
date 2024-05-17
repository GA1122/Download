static void _set_arm_thumb_bits(struct Elf_(r_bin_elf_obj_t) *bin, RBinSymbol **sym) {
	int bin_bits = Elf_(r_bin_elf_get_bits) (bin);
	RBinSymbol *ptr = *sym;
	int len = strlen (ptr->name);
	if (ptr->name[0] == '$' && (len >= 2 && !ptr->name[2])) {
		switch (ptr->name[1]) {
		case 'a' :  
			ptr->bits = 32;
			break;
		case 't':  
			ptr->bits = 16;
			if (ptr->vaddr & 1) {
				ptr->vaddr--;
			}
			if (ptr->paddr & 1) {
				ptr->paddr--;
			}
			break;
		case 'd':  
			break;
		default:
			goto arm_symbol;
		}
	} else {
arm_symbol:
		ptr->bits = bin_bits;
		if (bin_bits != 64) {
			ptr->bits = 32;
			if (ptr->vaddr & 1) {
				ptr->vaddr--;
				ptr->bits = 16;
			}
			if (ptr->paddr & 1) {
				ptr->paddr--;
				ptr->bits = 16;
			}
		}
	}
}