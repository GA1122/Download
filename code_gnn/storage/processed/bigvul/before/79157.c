static void kwajd_close(struct mskwaj_decompressor *base,
			struct mskwajd_header *hdr)
{
    struct mskwaj_decompressor_p *self = (struct mskwaj_decompressor_p *) base;
    struct mskwajd_header_p *hdr_p = (struct mskwajd_header_p *) hdr;

    if (!self || !self->system) return;

     
    self->system->close(hdr_p->fh);

     
    self->system->free(hdr);

    self->error = MSPACK_ERR_OK;
}
