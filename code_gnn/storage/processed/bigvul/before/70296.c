OJPEGReadHeaderInfoSecStreamDht(TIFF* tif)
{
	 
	 
	static const char module[]="OJPEGReadHeaderInfoSecStreamDht";
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint16 m;
	uint32 na;
	uint8* nb;
	uint8 o;
	if (OJPEGReadWord(sp,&m)==0)
		return(0);
	if (m<=2)
	{
		if (sp->subsamplingcorrect==0)
			TIFFErrorExt(tif->tif_clientdata,module,"Corrupt DHT marker in JPEG data");
		return(0);
	}
	if (sp->subsamplingcorrect!=0)
	{
		OJPEGReadSkip(sp,m-2);
	}
	else
	{
		na=sizeof(uint32)+2+m;
		nb=_TIFFmalloc(na);
		if (nb==0)
		{
			TIFFErrorExt(tif->tif_clientdata,module,"Out of memory");
			return(0);
		}
		*(uint32*)nb=na;
		nb[sizeof(uint32)]=255;
		nb[sizeof(uint32)+1]=JPEG_MARKER_DHT;
		nb[sizeof(uint32)+2]=(m>>8);
		nb[sizeof(uint32)+3]=(m&255);
		if (OJPEGReadBlock(sp,m-2,&nb[sizeof(uint32)+4])==0) {
                        _TIFFfree(nb);
			return(0);
                }
		o=nb[sizeof(uint32)+4];
		if ((o&240)==0)
		{
			if (3<o)
			{
				TIFFErrorExt(tif->tif_clientdata,module,"Corrupt DHT marker in JPEG data");
                                _TIFFfree(nb);
				return(0);
			}
			if (sp->dctable[o]!=0)
				_TIFFfree(sp->dctable[o]);
			sp->dctable[o]=nb;
		}
		else
		{
			if ((o&240)!=16)
			{
				TIFFErrorExt(tif->tif_clientdata,module,"Corrupt DHT marker in JPEG data");
                                _TIFFfree(nb);
				return(0);
			}
			o&=15;
			if (3<o)
			{
				TIFFErrorExt(tif->tif_clientdata,module,"Corrupt DHT marker in JPEG data");
                                _TIFFfree(nb);
				return(0);
			}
			if (sp->actable[o]!=0)
				_TIFFfree(sp->actable[o]);
			sp->actable[o]=nb;
		}
	}
	return(1);
}