static int init_hdr(struct MACH0_(obj_t)* bin) {
	ut8 magicbytes[4] = {0};
	ut8 machohdrbytes[sizeof (struct MACH0_(mach_header))] = {0};
	int len;

	if (r_buf_read_at (bin->b, 0, magicbytes, 4) < 1) {
		return false;
	}
	if (r_read_le32 (magicbytes) == 0xfeedface) {
		bin->big_endian = false;
	} else if (r_read_be32 (magicbytes) == 0xfeedface) {
		bin->big_endian = true;
	} else if (r_read_le32(magicbytes) == FAT_MAGIC) {
		bin->big_endian = false;
	} else if (r_read_be32(magicbytes) == FAT_MAGIC) {
		bin->big_endian = true;
	} else if (r_read_le32(magicbytes) == 0xfeedfacf) {
		bin->big_endian = false;
	} else if (r_read_be32(magicbytes) == 0xfeedfacf) {
		bin->big_endian = true;
	} else {
		return false;  
	}
	len = r_buf_read_at (bin->b, 0, machohdrbytes, sizeof (machohdrbytes));
	if (len != sizeof (machohdrbytes)) {
		bprintf ("Error: read (hdr)\n");
		return false;
	}
	bin->hdr.magic = r_read_ble (&machohdrbytes[0], bin->big_endian, 32);
	bin->hdr.cputype = r_read_ble (&machohdrbytes[4], bin->big_endian, 32);
	bin->hdr.cpusubtype = r_read_ble (&machohdrbytes[8], bin->big_endian, 32);
	bin->hdr.filetype = r_read_ble (&machohdrbytes[12], bin->big_endian, 32);
	bin->hdr.ncmds = r_read_ble (&machohdrbytes[16], bin->big_endian, 32);
	bin->hdr.sizeofcmds = r_read_ble (&machohdrbytes[20], bin->big_endian, 32);
	bin->hdr.flags = r_read_ble (&machohdrbytes[24], bin->big_endian, 32);
#if R_BIN_MACH064
	bin->hdr.reserved = r_read_ble (&machohdrbytes[28], bin->big_endian, 32);
#endif
	sdb_set (bin->kv, "mach0_header.format",
		"xxxxddx "
		"magic cputype cpusubtype filetype ncmds sizeofcmds flags", 0);
	sdb_num_set (bin->kv, "mach0_header.offset", 0, 0);  
	sdb_set (bin->kv, "mach_filetype.cparse", "enum mach_filetype{MH_OBJECT=1,"
			"MH_EXECUTE=2, MH_FVMLIB=3, MH_CORE=4, MH_PRELOAD=5, MH_DYLIB=6,"
			"MH_DYLINKER=7, MH_BUNDLE=8, MH_DYLIB_STUB=9, MH_DSYM=10,"
			"MH_KEXT_BUNDLE=11}"
			,0);
	sdb_set (bin->kv, "mach_flags.cparse", "enum mach_flags{MH_NOUNDEFS=1,"
			"MH_INCRLINK=2,MH_DYLDLINK=4,MH_BINDATLOAD=8,MH_PREBOUND=0x10,"
			"MH_SPLIT_SEGS=0x20,MH_LAZY_INIT=0x40,MH_TWOLEVEL=0x80,"
			"MH_FORCE_FLAT=0x100,MH_NOMULTIDEFS=0x200,MH_NOFIXPREBINDING=0x400,"
			"MH_PREBINDABLE=0x800, MH_ALLMODSBOUND=0x1000,"
			"MH_SUBSECTIONS_VIA_SYMBOLS=0x2000,"
			"MH_CANONICAL=0x4000,MH_WEAK_DEFINES=0x8000,"
			"MH_BINDS_TO_WEAK=0x10000,MH_ALLOW_STACK_EXECUTION=0x20000,"
			"MH_ROOT_SAFE=0x40000,MH_SETUID_SAFE=0x80000,"
			"MH_NO_REEXPORTED_DYLIBS=0x100000,MH_PIE=0x200000,"
			"MH_DEAD_STRIPPABLE_DYLIB=0x400000,"
			"MH_HAS_TLV_DESCRIPTORS=0x800000,"
			"MH_NO_HEAP_EXECUTION=0x1000000 }",0);
	return true;
}