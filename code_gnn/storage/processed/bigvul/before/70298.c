OJPEGReadHeaderInfoSecStreamDri(TIFF* tif)
{
	 
	static const char module[]="OJPEGReadHeaderInfoSecStreamDri";
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint16 m;
	if (OJPEGReadWord(sp,&m)==0)
		return(0);
	if (m!=4)
	{
		TIFFErrorExt(tif->tif_clientdata,module,"Corrupt DRI marker in JPEG data");
		return(0);
	}
	if (OJPEGReadWord(sp,&m)==0)
		return(0);
	sp->restart_interval=m;
	return(1);
}
