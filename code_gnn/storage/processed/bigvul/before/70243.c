LogLuvEncode32(TIFF* tif, uint8* bp, tmsize_t cc, uint16 s)
{
	static const char module[] = "LogLuvEncode32";
	LogLuvState* sp = EncoderState(tif);
	int shft;
	tmsize_t i;
	tmsize_t j;
	tmsize_t npixels;
	uint8* op;
	uint32* tp;
	uint32 b;
	tmsize_t occ;
	int rc=0, mask;
	tmsize_t beg;

	assert(s == 0);
	assert(sp != NULL);

	npixels = cc / sp->pixel_size;

	if (sp->user_datafmt == SGILOGDATAFMT_RAW)
		tp = (uint32*) bp;
	else {
		tp = (uint32*) sp->tbuf;
		if(sp->tbuflen < npixels) {
			TIFFErrorExt(tif->tif_clientdata, module,
						 "Translation buffer too short");
			return (0);
		}
		(*sp->tfunc)(sp, bp, npixels);
	}
	 
	op = tif->tif_rawcp;
	occ = tif->tif_rawdatasize - tif->tif_rawcc;
	for (shft = 4*8; (shft -= 8) >= 0; )
		for (i = 0; i < npixels; i += rc) {
			if (occ < 4) {
				tif->tif_rawcp = op;
				tif->tif_rawcc = tif->tif_rawdatasize - occ;
				if (!TIFFFlushData1(tif))
					return (-1);
				op = tif->tif_rawcp;
				occ = tif->tif_rawdatasize - tif->tif_rawcc;
			}
			mask = 0xff << shft;		 
			for (beg = i; beg < npixels; beg += rc) {
				b = tp[beg] & mask;
				rc = 1;
				while (rc < 127+2 && beg+rc < npixels &&
						(tp[beg+rc] & mask) == b)
					rc++;
				if (rc >= MINRUN)
					break;		 
			}
			if (beg-i > 1 && beg-i < MINRUN) {
				b = tp[i] & mask;	 
				j = i+1;
				while ((tp[j++] & mask) == b)
					if (j == beg) {
						*op++ = (uint8)(128-2+j-i);
						*op++ = (uint8)(b >> shft);
						occ -= 2;
						i = beg;
						break;
					}
			}
			while (i < beg) {		 
				if ((j = beg-i) > 127) j = 127;
				if (occ < j+3) {
					tif->tif_rawcp = op;
					tif->tif_rawcc = tif->tif_rawdatasize - occ;
					if (!TIFFFlushData1(tif))
						return (-1);
					op = tif->tif_rawcp;
					occ = tif->tif_rawdatasize - tif->tif_rawcc;
				}
				*op++ = (uint8) j; occ--;
				while (j--) {
					*op++ = (uint8)(tp[i++] >> shft & 0xff);
					occ--;
				}
			}
			if (rc >= MINRUN) {		 
				*op++ = (uint8) (128-2+rc);
				*op++ = (uint8)(tp[beg] >> shft & 0xff);
				occ -= 2;
			} else
				rc = 0;
		}
	tif->tif_rawcp = op;
	tif->tif_rawcc = tif->tif_rawdatasize - occ;

	return (1);
}