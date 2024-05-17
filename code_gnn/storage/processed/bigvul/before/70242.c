LogLuvEncode24(TIFF* tif, uint8* bp, tmsize_t cc, uint16 s)
{
	static const char module[] = "LogLuvEncode24";
	LogLuvState* sp = EncoderState(tif);
	tmsize_t i;
	tmsize_t npixels;
	tmsize_t occ;
	uint8* op;
	uint32* tp;

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
	for (i = npixels; i--; ) {
		if (occ < 3) {
			tif->tif_rawcp = op;
			tif->tif_rawcc = tif->tif_rawdatasize - occ;
			if (!TIFFFlushData1(tif))
				return (-1);
			op = tif->tif_rawcp;
			occ = tif->tif_rawdatasize - tif->tif_rawcc;
		}
		*op++ = (uint8)(*tp >> 16);
		*op++ = (uint8)(*tp >> 8 & 0xff);
		*op++ = (uint8)(*tp++ & 0xff);
		occ -= 3;
	}
	tif->tif_rawcp = op;
	tif->tif_rawcc = tif->tif_rawdatasize - occ;

	return (1);
}