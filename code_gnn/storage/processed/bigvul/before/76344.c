static int read_spaninfo(struct mschm_decompressor_p *self,
			 struct mschmd_sec_mscompressed *sec,
			 off_t *length_ptr)
{
    struct mspack_system *sys = self->system;
    unsigned char *data;
    
     
    int err = find_sys_file(self, sec, &sec->spaninfo, spaninfo_name);
    if (err) return MSPACK_ERR_DATAFORMAT;

     
    if (sec->spaninfo->length != 8) {
	D(("SpanInfo file is wrong size"))
	return MSPACK_ERR_DATAFORMAT;
    }

     
    if (!(data = read_sys_file(self, sec->spaninfo))) {
	D(("can't read SpanInfo file"))
	return self->error;
    }

     
    err = read_off64(length_ptr, data, sys, self->d->infh);
    sys->free(data);
    if (err) return MSPACK_ERR_DATAFORMAT;

    if (*length_ptr <= 0) {
        D(("output length is invalid"))
        return MSPACK_ERR_DATAFORMAT;
    }

    return MSPACK_ERR_OK;
}
