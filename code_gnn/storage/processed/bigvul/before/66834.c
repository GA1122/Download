void MACH0_(mach_headerfields)(RBinFile *file) {
	RBuffer *buf = file->buf;
	int n = 0;
	struct MACH0_(mach_header) *mh = MACH0_(get_hdr_from_bytes)(buf);
	eprintf ("0x00000000  Magic       0x%x\n", mh->magic);
	eprintf ("0x00000004  CpuType     0x%x\n", mh->cputype);
	eprintf ("0x00000008  CpuSubType  0x%x\n", mh->cpusubtype);
	eprintf ("0x0000000c  FileType    0x%x\n", mh->filetype);
	eprintf ("0x00000010  nCmds       %d\n", mh->ncmds);
	eprintf ("0x00000014  sizeOfCmds  %d\n", mh->sizeofcmds);
	eprintf ("0x00000018  Flags       0x%x\n", mh->flags);

	ut64 addr = 0x20 - 4;
	ut32 word = 0;
	ut8 wordbuf[sizeof(word)];
#define READWORD() \
		addr += 4; \
		if (!r_buf_read_at (buf, addr, (ut8*)wordbuf, 4)) { \
			eprintf ("Invalid address in buffer."); \
			break; \
		} \
		word = r_read_le32 (wordbuf);
	for (n = 0; n < mh->ncmds; n++) {
		eprintf ("\nLoad Command %d\n", n);
		READWORD();
		eprintf ("0x%08"PFMT64x"  cmd          0x%x %s\n",
			addr, word, cmd_to_string (word));
		READWORD();
		word &= 0xFFFFFF;
		eprintf ("0x%08"PFMT64x"  cmdsize      %d\n", addr, word);
		if ((int)(word) < 1) {
			eprintf ("Invalid size\n");
			break;
		}
		addr += word - 8;
	}
}
