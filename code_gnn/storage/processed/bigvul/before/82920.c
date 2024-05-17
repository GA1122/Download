static RBuffer* create(RBin* bin, const ut8 *code, int codelen, const ut8 *data, int datalen) {
	ut32 filesize, code_va, code_pa, phoff;
	ut32 p_start, p_phoff, p_phdr;
	ut32 p_ehdrsz, p_phdrsz;
	ut16 ehdrsz, phdrsz;
	ut32 p_vaddr, p_paddr, p_fs, p_fs2;
	ut32 baddr;
	int is_arm = 0;
	RBuffer *buf = r_buf_new ();
	if (bin && bin->cur && bin->cur->o && bin->cur->o->info) {
		is_arm = !strcmp (bin->cur->o->info->arch, "arm");
	}
	if (is_arm) {
		baddr = 0x40000;
	} else {
		baddr = 0x8048000;
	}

#define B(x,y) r_buf_append_bytes(buf,(const ut8*)x,y)
#define D(x) r_buf_append_ut32(buf,x)
#define H(x) r_buf_append_ut16(buf,x)
#define Z(x) r_buf_append_nbytes(buf,x)
#define W(x,y,z) r_buf_write_at(buf,x,(const ut8*)y,z)
#define WZ(x,y) p_tmp=buf->length;Z(x);W(p_tmp,y,strlen(y))

	B ("\x7F" "ELF" "\x01\x01\x01\x00", 8);
	Z (8);
	H (2);  
	if (is_arm) {
		H (40);  
	} else {
		H (3);  
	}

	D (1);
	p_start = buf->length;
	D (-1);  
	p_phoff = buf->length;
	D (-1);  
	D (0);   
	D (0);   
	p_ehdrsz = buf->length;
	H (-1);  
	p_phdrsz = buf->length;
	H (-1);  
	H (1);
	H (0);
	H (0);
	H (0);
	p_phdr = buf->length;
	D (1);
	D (0);
	p_vaddr = buf->length;
	D (-1);  
	p_paddr = buf->length;
	D (-1);  
	p_fs = buf->length;
	D (-1);  
	p_fs2 = buf->length;
	D (-1);  
	D (5);  
	D (0x1000);  

	ehdrsz = p_phdr;
	phdrsz = buf->length - p_phdr;
	code_pa = buf->length;
	code_va = code_pa + baddr;
	phoff = 0x34; 
	filesize = code_pa + codelen + datalen;

	W (p_start, &code_va, 4);
	W (p_phoff, &phoff, 4);
	W (p_ehdrsz, &ehdrsz, 2);
	W (p_phdrsz, &phdrsz, 2);

	code_va = baddr;  
	W (p_vaddr, &code_va, 4);
	code_pa = baddr;  
	W (p_paddr, &code_pa, 4);

	W (p_fs, &filesize, 4);
	W (p_fs2, &filesize, 4);

	B (code, codelen);

	if (data && datalen > 0) {
		eprintf ("Warning: DATA section not support for ELF yet\n");
		B (data, datalen);
	}
	return buf;
}
