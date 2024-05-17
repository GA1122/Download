_TIFFDefaultStripSize(TIFF* tif, uint32 s)
{
	if ((int32) s < 1) {
		 
		uint64 scanlinesize;
		uint64 rows;
		scanlinesize=TIFFScanlineSize64(tif);
		if (scanlinesize==0)
			scanlinesize=1;
		rows=(uint64)STRIP_SIZE_DEFAULT/scanlinesize;
		if (rows==0)
			rows=1;
		else if (rows>0xFFFFFFFF)
			rows=0xFFFFFFFF;
		s=(uint32)rows;
	}
	return (s);
}
