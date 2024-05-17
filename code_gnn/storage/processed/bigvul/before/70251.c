LogLuvVSetField(TIFF* tif, uint32 tag, va_list ap)
{
	static const char module[] = "LogLuvVSetField";
	LogLuvState* sp = DecoderState(tif);
	int bps, fmt;

	switch (tag) {
	case TIFFTAG_SGILOGDATAFMT:
		sp->user_datafmt = (int) va_arg(ap, int);
		 
		switch (sp->user_datafmt) {
		case SGILOGDATAFMT_FLOAT:
			bps = 32;
			fmt = SAMPLEFORMAT_IEEEFP;
			break;
		case SGILOGDATAFMT_16BIT:
			bps = 16;
			fmt = SAMPLEFORMAT_INT;
			break;
		case SGILOGDATAFMT_RAW:
			bps = 32;
			fmt = SAMPLEFORMAT_UINT;
			TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
			break;
		case SGILOGDATAFMT_8BIT:
			bps = 8;
			fmt = SAMPLEFORMAT_UINT;
			break;
		default:
			TIFFErrorExt(tif->tif_clientdata, tif->tif_name,
			    "Unknown data format %d for LogLuv compression",
			    sp->user_datafmt);
			return (0);
		}
		TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bps);
		TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, fmt);
		 
		tif->tif_tilesize = isTiled(tif) ? TIFFTileSize(tif) : (tmsize_t) -1;
		tif->tif_scanlinesize = TIFFScanlineSize(tif);
		return (1);
	case TIFFTAG_SGILOGENCODE:
		sp->encode_meth = (int) va_arg(ap, int);
		if (sp->encode_meth != SGILOGENCODE_NODITHER &&
		    sp->encode_meth != SGILOGENCODE_RANDITHER) {
			TIFFErrorExt(tif->tif_clientdata, module,
			    "Unknown encoding %d for LogLuv compression",
			    sp->encode_meth);
			return (0);
		}
		return (1);
	default:
		return (*sp->vsetparent)(tif, tag, ap);
	}
}