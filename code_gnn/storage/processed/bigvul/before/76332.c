static int chmd_fast_find(struct mschm_decompressor *base,
			  struct mschmd_header *chm, const char *filename,
			  struct mschmd_file *f_ptr, int f_size)
{
    struct mschm_decompressor_p *self = (struct mschm_decompressor_p *) base;
    struct mspack_system *sys;
    struct mspack_file *fh;
    const unsigned char *chunk, *p, *end;
    int err = MSPACK_ERR_OK, result = -1;
    unsigned int n, sec;

    if (!self || !chm || !f_ptr || (f_size != sizeof(struct mschmd_file))) {
	return MSPACK_ERR_ARGS;
    }
    sys = self->system;

     
    memset(f_ptr, 0, f_size);

    if (!(fh = sys->open(sys, chm->filename, MSPACK_SYS_OPEN_READ))) {
	return MSPACK_ERR_OPEN;
    }

     
    if (chm->index_root < chm->num_chunks) {
	n = chm->index_root;
	for (;;) {
	    if (!(chunk = read_chunk(self, chm, fh, n))) {
		sys->close(fh);
		return self->error;
	    }

	     
	    if ((result = search_chunk(chm, chunk, filename, &p, &end)) <= 0) {
		break;
	    }

	     
	    if (chunk[3] == 0x4C) break; else READ_ENCINT(n);
	}
    }
    else {
	 
	for (n = chm->first_pmgl; n <= chm->last_pmgl;
	     n = EndGetI32(&chunk[pmgl_NextChunk]))
	{
	    if (!(chunk = read_chunk(self, chm, fh, n))) {
		err = self->error;
		break;
	    }

	     
	    if ((result = search_chunk(chm, chunk, filename, &p, &end)) > 0) {
		break;
	    }

             
            if ((int)n == EndGetI32(&chunk[pmgl_NextChunk])) {
                break;
            }
	}
    }

     
    if (result > 0) {
	READ_ENCINT(sec);
	f_ptr->section  = (sec == 0) ? (struct mschmd_section *) &chm->sec0
	                             : (struct mschmd_section *) &chm->sec1;
	READ_ENCINT(f_ptr->offset);
	READ_ENCINT(f_ptr->length);
    }
    else if (result < 0) {
	err = MSPACK_ERR_DATAFORMAT;
    }

    sys->close(fh);
    return self->error = err;

 chunk_end:
    D(("read beyond end of chunk entries"))
    sys->close(fh);
    return self->error = MSPACK_ERR_DATAFORMAT;
}
