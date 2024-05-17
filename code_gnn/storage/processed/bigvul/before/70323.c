TIFFInitOJPEG(TIFF* tif, int scheme)
{
	static const char module[]="TIFFInitOJPEG";
	OJPEGState* sp;

	assert(scheme==COMPRESSION_OJPEG);

         
	if (!_TIFFMergeFields(tif, ojpegFields, TIFFArrayCount(ojpegFields))) {
		TIFFErrorExt(tif->tif_clientdata, module,
		    "Merging Old JPEG codec-specific tags failed");
		return 0;
	}

	 
	sp=_TIFFmalloc(sizeof(OJPEGState));
	if (sp==NULL)
	{
		TIFFErrorExt(tif->tif_clientdata,module,"No space for OJPEG state block");
		return(0);
	}
	_TIFFmemset(sp,0,sizeof(OJPEGState));
	sp->tif=tif;
	sp->jpeg_proc=1;
	sp->subsampling_hor=2;
	sp->subsampling_ver=2;
	TIFFSetField(tif,TIFFTAG_YCBCRSUBSAMPLING,2,2);
	 
	tif->tif_fixuptags=OJPEGFixupTags;  
	tif->tif_setupdecode=OJPEGSetupDecode;
	tif->tif_predecode=OJPEGPreDecode;
	tif->tif_postdecode=OJPEGPostDecode;  
	tif->tif_decoderow=OJPEGDecode;  
	tif->tif_decodestrip=OJPEGDecode;  
	tif->tif_decodetile=OJPEGDecode;  
	tif->tif_setupencode=OJPEGSetupEncode;
	tif->tif_preencode=OJPEGPreEncode;
	tif->tif_postencode=OJPEGPostEncode;
	tif->tif_encoderow=OJPEGEncode;  
	tif->tif_encodestrip=OJPEGEncode;  
	tif->tif_encodetile=OJPEGEncode;  
	tif->tif_cleanup=OJPEGCleanup;
	tif->tif_data=(uint8*)sp;
	 
	sp->vgetparent=tif->tif_tagmethods.vgetfield;
	tif->tif_tagmethods.vgetfield=OJPEGVGetField;
	sp->vsetparent=tif->tif_tagmethods.vsetfield;
	tif->tif_tagmethods.vsetfield=OJPEGVSetField;
	sp->printdir=tif->tif_tagmethods.printdir;
	tif->tif_tagmethods.printdir=OJPEGPrintDir;
	 
	tif->tif_flags|=TIFF_NOREADRAW;
	return(1);
}
