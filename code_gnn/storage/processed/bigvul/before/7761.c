static int h2_frt_transfer_data(struct h2s *h2s, struct buffer *buf, int count)
{
	struct h2c *h2c = h2s->h2c;
	int block1, block2;
	unsigned int flen = h2c->dfl;

	h2s->cs->flags &= ~CS_FL_RCV_MORE;
	h2c->flags &= ~H2_CF_DEM_SFULL;

	 
	if (h2c->dff & H2_F_DATA_PADDED) {
		if (h2c->dbuf->i < 1)
			return 0;

		h2c->dpl = *(uint8_t *)bi_ptr(h2c->dbuf);
		if (h2c->dpl >= h2c->dfl) {
			 
			h2c_error(h2c, H2_ERR_PROTOCOL_ERROR);
			return 0;
		}

		 
		bi_del(h2c->dbuf, 1);
		h2c->dfl--;
		h2c->rcvd_c++; h2c->rcvd_s++;
		h2c->dff &= ~H2_F_DATA_PADDED;
	}

	flen = h2c->dfl - h2c->dpl;
	if (!flen)
		goto end_transfer;

	if (flen > h2c->dbuf->i) {
		flen = h2c->dbuf->i;
		if (!flen)
			return 0;
	}

	 
	if (flen > count) {
		flen = count;
		if (!flen) {
			h2c->flags |= H2_CF_DEM_SFULL;
			h2s->cs->flags |= CS_FL_RCV_MORE;
			return 0;
		}
	}

	 
	block1 = bi_contig_data(h2c->dbuf);
	if (block1 > flen)
		block1 = flen;
	block2 = flen - block1;

	if (block1)
		bi_putblk(buf, b_ptr(h2c->dbuf, 0), block1);

	if (block2)
		bi_putblk(buf, b_ptr(h2c->dbuf, block1), block2);

	 
	bi_del(h2c->dbuf, flen);
	h2c->dfl    -= flen;
	h2c->rcvd_c += flen;
	h2c->rcvd_s += flen;   

	if (h2c->dfl > h2c->dpl) {
		 
		h2c->flags |= H2_CF_DEM_SFULL;
		h2s->cs->flags |= CS_FL_RCV_MORE;
		return flen;
	}

 end_transfer:
	 
	h2c->rcvd_c += h2c->dpl;
	h2c->rcvd_s += h2c->dpl;
	h2c->dpl = 0;
	h2c->st0 = H2_CS_FRAME_A;  

	 
	if (h2c->dff & H2_F_DATA_END_STREAM) {
		h2s->cs->flags |= CS_FL_EOS;
		h2s->flags |= H2_SF_ES_RCVD;
	}

	return flen;
}
