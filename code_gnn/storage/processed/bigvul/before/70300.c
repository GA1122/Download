OJPEGReadHeaderInfoSecStreamSos(TIFF* tif)
{
	 
	static const char module[]="OJPEGReadHeaderInfoSecStreamSos";
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint16 m;
	uint8 n;
	uint8 o;
	assert(sp->subsamplingcorrect==0);
	if (sp->sof_log==0)
	{
		TIFFErrorExt(tif->tif_clientdata,module,"Corrupt SOS marker in JPEG data");
		return(0);
	}
	 
	if (OJPEGReadWord(sp,&m)==0)
		return(0);
	if (m!=6+sp->samples_per_pixel_per_plane*2)
	{
		TIFFErrorExt(tif->tif_clientdata,module,"Corrupt SOS marker in JPEG data");
		return(0);
	}
	 
	if (OJPEGReadByte(sp,&n)==0)
		return(0);
	if (n!=sp->samples_per_pixel_per_plane)
	{
		TIFFErrorExt(tif->tif_clientdata,module,"Corrupt SOS marker in JPEG data");
		return(0);
	}
	 
	for (o=0; o<sp->samples_per_pixel_per_plane; o++)
	{
		 
		if (OJPEGReadByte(sp,&n)==0)
			return(0);
		sp->sos_cs[sp->plane_sample_offset+o]=n;
		 
		if (OJPEGReadByte(sp,&n)==0)
			return(0);
		sp->sos_tda[sp->plane_sample_offset+o]=n;
	}
	 
	OJPEGReadSkip(sp,3);
	return(1);
}
