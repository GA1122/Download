TIFFInitSGILog(TIFF* tif, int scheme)
{
	static const char module[] = "TIFFInitSGILog";
	LogLuvState* sp;

	assert(scheme == COMPRESSION_SGILOG24 || scheme == COMPRESSION_SGILOG);

	 
	if (!_TIFFMergeFields(tif, LogLuvFields,
			      TIFFArrayCount(LogLuvFields))) {
		TIFFErrorExt(tif->tif_clientdata, module,
		    "Merging SGILog codec-specific tags failed");
		return 0;
	}

	 
	tif->tif_data = (uint8*) _TIFFmalloc(sizeof (LogLuvState));
	if (tif->tif_data == NULL)
		goto bad;
	sp = (LogLuvState*) tif->tif_data;
	_TIFFmemset((void*)sp, 0, sizeof (*sp));
	sp->user_datafmt = SGILOGDATAFMT_UNKNOWN;
	sp->encode_meth = (scheme == COMPRESSION_SGILOG24) ?
	    SGILOGENCODE_RANDITHER : SGILOGENCODE_NODITHER;
	sp->tfunc = _logLuvNop;

	 
	tif->tif_fixuptags = LogLuvFixupTags;  
	tif->tif_setupdecode = LogLuvSetupDecode;
	tif->tif_decodestrip = LogLuvDecodeStrip;
	tif->tif_decodetile = LogLuvDecodeTile;
	tif->tif_setupencode = LogLuvSetupEncode;
	tif->tif_encodestrip = LogLuvEncodeStrip;  
	tif->tif_encodetile = LogLuvEncodeTile;
	tif->tif_close = LogLuvClose;
	tif->tif_cleanup = LogLuvCleanup;

	 
	sp->vgetparent = tif->tif_tagmethods.vgetfield;
	tif->tif_tagmethods.vgetfield = LogLuvVGetField;    
	sp->vsetparent = tif->tif_tagmethods.vsetfield;
	tif->tif_tagmethods.vsetfield = LogLuvVSetField;    

	return (1);
bad:
	TIFFErrorExt(tif->tif_clientdata, module,
		     "%s: No space for LogLuv state block", tif->tif_name);
	return (0);
}
