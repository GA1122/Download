  struct r_bin_dyldcache_lib_t *r_bin_dyldcache_extract(struct r_bin_dyldcache_obj_t* bin, int idx, int *nlib) {
  	ut64 liboff, linkedit_offset;
 	ut64 dyld_vmbase;
 	ut32 addend = 0;
 	struct r_bin_dyldcache_lib_t *ret = NULL;
 	struct dyld_cache_image_info* image_infos = NULL;
 	struct mach_header *mh;
 	ut8 *data, *cmdptr;
 	int cmd, libsz = 0;
 	RBuffer* dbuf;
 	char *libname;
 
 	if (!bin) {
 		return NULL;
 	}
 	if (bin->size < 1) {
 		eprintf ("Empty file? (%s)\n", bin->file? bin->file: "(null)");
 		return NULL;
 	}
 	if (bin->nlibs < 0 || idx < 0 || idx >= bin->nlibs) {
 		return NULL;
 	}
  	*nlib = bin->nlibs;
  	ret = R_NEW0 (struct r_bin_dyldcache_lib_t);
  	if (!ret) {
		perror ("malloc (ret)");
  		return NULL;
  	}
  	if (bin->hdr.startaddr > bin->size) {
  	    	eprintf ("corrupted dyldcache");
  		free (ret);
  		return NULL;
  	}
// 
  	if (bin->hdr.startaddr > bin->size || bin->hdr.baseaddroff > bin->size) {
  		eprintf ("corrupted dyldcache");
  		free (ret);
  		return NULL;
  	}
	image_infos = (struct dyld_cache_image_info*) (bin->b->buf + bin->hdr.startaddr);
	dyld_vmbase = *(ut64 *)(bin->b->buf + bin->hdr.baseaddroff);
// 	int sz = bin->nlibs * sizeof (struct dyld_cache_image_info);
// 	image_infos = malloc (sz);  
// 	if (!image_infos) {
// 		free (ret);
// 		return NULL;
// 	}
// 	r_buf_read_at (bin->b, bin->hdr.startaddr, (ut8*)image_infos, sz);
// 	dyld_vmbase = r_buf_read64le (bin->b, bin->hdr.baseaddroff);
  	liboff = image_infos[idx].address - dyld_vmbase;
  	if (liboff > bin->size) {
  		eprintf ("Corrupted file\n");
  		free (ret);
  		return NULL;
  	}
  	ret->offset = liboff;
	if (image_infos[idx].pathFileOffset > bin->size) {
	    eprintf ("corrupted file\n");
// 	int pfo = image_infos[idx].pathFileOffset;
// 	if (pfo < 0 || pfo > bin->size) {
// 		eprintf ("corrupted file: pathFileOffset > bin->size (%d)\n", pfo);
  		free (ret);
  		return NULL;
  	}
	libname = (char *)(bin->b->buf + image_infos[idx].pathFileOffset);
// 	libname = r_buf_read_string (bin->b, pfo, 64);
  	 
  	data = bin->b->buf + liboff;
  	mh = (struct mach_header *)data;
 	 
 	if (mh->magic != MH_MAGIC && mh->magic != MH_MAGIC_64) {
 		if (mh->magic == 0xbebafeca) {  
 			eprintf ("FAT Binary\n");
 		}
 		eprintf ("Not mach-o\n");
 		free (ret);
 		return NULL;
 	}
 	 
 	if (!(dbuf = r_buf_new ())) {
 		eprintf ("new (dbuf)\n");
 		free (ret);
 		return NULL;
 	}
 	addend = mh->magic == MH_MAGIC? sizeof (struct mach_header) : sizeof (struct mach_header_64);
 	r_buf_set_bytes (dbuf, data, addend);
 	cmdptr = data + addend;
 	 
 	for (cmd = 0; cmd < mh->ncmds; cmd++) {
 		struct load_command *lc = (struct load_command *)cmdptr;
 		r_buf_append_bytes (dbuf, (ut8*)lc, lc->cmdsize);
 		cmdptr += lc->cmdsize;
 	}
 	cmdptr = data + addend;
 	 
 	for (cmd = linkedit_offset = 0; cmd < mh->ncmds; cmd++) {
 		struct load_command *lc = (struct load_command *)cmdptr;
 		cmdptr += lc->cmdsize;
 		switch (lc->cmd) {
 		case LC_SEGMENT:
 			{
 			 
 			struct segment_command *seg = (struct segment_command *)lc;
 			int t = seg->filesize;
 			if (seg->fileoff + seg->filesize > bin->size || seg->fileoff > bin->size) {
 				eprintf ("malformed dyldcache\n");
 				free (ret);
 				r_buf_free (dbuf);
 				return NULL;
 			}
 			r_buf_append_bytes (dbuf, bin->b->buf+seg->fileoff, t);
 			r_bin_dyldcache_apply_patch (dbuf, dbuf->length, (ut64)((size_t)&seg->fileoff - (size_t)data));
 			 
 			int sect_offset = seg->fileoff - libsz;
 			libsz = dbuf->length;
 			if (!strcmp (seg->segname, "__LINKEDIT")) {
 				linkedit_offset = sect_offset;
 			}
 			if (seg->nsects > 0) {
 				struct section *sects = (struct section *)((ut8 *)seg + sizeof(struct segment_command));
 				int nsect;
 				for (nsect = 0; nsect < seg->nsects; nsect++) {
 					if (sects[nsect].offset > libsz) {
 						r_bin_dyldcache_apply_patch (dbuf, sects[nsect].offset - sect_offset,
 							(ut64)((size_t)&sects[nsect].offset - (size_t)data));
 					}
 				}
 			}
 			}
 			break;
 		case LC_SYMTAB:
 			{
 			struct symtab_command *st = (struct symtab_command *)lc;
 			NZ_OFFSET (st->symoff);
 			NZ_OFFSET (st->stroff);
 			}
 			break;
 		case LC_DYSYMTAB:
 			{
 			struct dysymtab_command *st = (struct dysymtab_command *)lc;
 			NZ_OFFSET (st->tocoff);
 			NZ_OFFSET (st->modtaboff);
 			NZ_OFFSET (st->extrefsymoff);
 			NZ_OFFSET (st->indirectsymoff);
 			NZ_OFFSET (st->extreloff);
 			NZ_OFFSET (st->locreloff);
 			}
 			break;
 		case LC_DYLD_INFO:
 		case LC_DYLD_INFO_ONLY:
 			{
 			struct dyld_info_command *st = (struct dyld_info_command *)lc;
 			NZ_OFFSET (st->rebase_off);
 			NZ_OFFSET (st->bind_off);
 			NZ_OFFSET (st->weak_bind_off);
 			NZ_OFFSET (st->lazy_bind_off);
 			NZ_OFFSET (st->export_off);
 			}
 			break;
 		}
 	}
 	 
 	ret->b = dbuf;
 	strncpy (ret->path, libname, sizeof (ret->path) - 1);
 	ret->size = libsz;
 	return ret;
 }