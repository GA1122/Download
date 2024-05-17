static RBinAddr* newEntry(ut64 haddr, ut64 paddr, int type, int bits) {
	RBinAddr *ptr = R_NEW0 (RBinAddr);
	if (ptr) {
		ptr->paddr = paddr;
		ptr->vaddr = paddr;
		ptr->haddr = haddr;
		ptr->bits = bits;
		ptr->type = type;
		if (bits == 16 && ptr->vaddr & 1) {
			ptr->paddr--;
			ptr->vaddr--;
		}
	}
	return ptr;
}