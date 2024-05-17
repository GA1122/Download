LogL16InitState(TIFF* tif)
{
	static const char module[] = "LogL16InitState";
	TIFFDirectory *td = &tif->tif_dir;
	LogLuvState* sp = DecoderState(tif);

	assert(sp != NULL);
	assert(td->td_photometric == PHOTOMETRIC_LOGL);

	if( td->td_samplesperpixel != 1 )
	{
		TIFFErrorExt(tif->tif_clientdata, module,
		             "Sorry, can not handle LogL image with %s=%d",
			     "Samples/pixel", td->td_samplesperpixel);
		return 0;
	}

	 
	if (sp->user_datafmt == SGILOGDATAFMT_UNKNOWN)
		sp->user_datafmt = LogL16GuessDataFmt(td);
	switch (sp->user_datafmt) {
	case SGILOGDATAFMT_FLOAT:
		sp->pixel_size = sizeof (float);
		break;
	case SGILOGDATAFMT_16BIT:
		sp->pixel_size = sizeof (int16);
		break;
	case SGILOGDATAFMT_8BIT:
		sp->pixel_size = sizeof (uint8);
		break;
	default:
		TIFFErrorExt(tif->tif_clientdata, module,
		    "No support for converting user data format to LogL");
		return (0);
	}
        if( isTiled(tif) )
            sp->tbuflen = multiply_ms(td->td_tilewidth, td->td_tilelength);
        else
            sp->tbuflen = multiply_ms(td->td_imagewidth, td->td_rowsperstrip);
	if (multiply_ms(sp->tbuflen, sizeof (int16)) == 0 ||
	    (sp->tbuf = (uint8*) _TIFFmalloc(sp->tbuflen * sizeof (int16))) == NULL) {
		TIFFErrorExt(tif->tif_clientdata, module, "No space for SGILog translation buffer");
		return (0);
	}
	return (1);
}