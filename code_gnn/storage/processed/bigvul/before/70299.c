OJPEGReadHeaderInfoSecStreamSof(TIFF* tif, uint8 marker_id)
{
	 
	static const char module[]="OJPEGReadHeaderInfoSecStreamSof";
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint16 m;
	uint16 n;
	uint8 o;
	uint16 p;
	uint16 q;
	if (sp->sof_log!=0)
	{
		TIFFErrorExt(tif->tif_clientdata,module,"Corrupt JPEG data");
		return(0);
	}
	if (sp->subsamplingcorrect==0)
		sp->sof_marker_id=marker_id;
	 
	if (OJPEGReadWord(sp,&m)==0)
		return(0);
	if (m<11)
	{
		if (sp->subsamplingcorrect==0)
			TIFFErrorExt(tif->tif_clientdata,module,"Corrupt SOF marker in JPEG data");
		return(0);
	}
	m-=8;
	if (m%3!=0)
	{
		if (sp->subsamplingcorrect==0)
			TIFFErrorExt(tif->tif_clientdata,module,"Corrupt SOF marker in JPEG data");
		return(0);
	}
	n=m/3;
	if (sp->subsamplingcorrect==0)
	{
		if (n!=sp->samples_per_pixel)
		{
			TIFFErrorExt(tif->tif_clientdata,module,"JPEG compressed data indicates unexpected number of samples");
			return(0);
		}
	}
	 
	if (OJPEGReadByte(sp,&o)==0)
		return(0);
	if (o!=8)
	{
		if (sp->subsamplingcorrect==0)
			TIFFErrorExt(tif->tif_clientdata,module,"JPEG compressed data indicates unexpected number of bits per sample");
		return(0);
	}
	 
	if (sp->subsamplingcorrect)
		OJPEGReadSkip(sp,4);
	else
	{
		 
		if (OJPEGReadWord(sp,&p)==0)
			return(0);
		if (((uint32)p<sp->image_length) && ((uint32)p<sp->strile_length_total))
		{
			TIFFErrorExt(tif->tif_clientdata,module,"JPEG compressed data indicates unexpected height");
			return(0);
		}
		sp->sof_y=p;
		 
		if (OJPEGReadWord(sp,&p)==0)
			return(0);
		if (((uint32)p<sp->image_width) && ((uint32)p<sp->strile_width))
		{
			TIFFErrorExt(tif->tif_clientdata,module,"JPEG compressed data indicates unexpected width");
			return(0);
		}
		if ((uint32)p>sp->strile_width)
		{
			TIFFErrorExt(tif->tif_clientdata,module,"JPEG compressed data image width exceeds expected image width");
			return(0);
		}
		sp->sof_x=p;
	}
	 
	if (OJPEGReadByte(sp,&o)==0)
		return(0);
	if (o!=n)
	{
		if (sp->subsamplingcorrect==0)
			TIFFErrorExt(tif->tif_clientdata,module,"Corrupt SOF marker in JPEG data");
		return(0);
	}
	 
	 
	for (q=0; q<n; q++)
	{
		 
		if (OJPEGReadByte(sp,&o)==0)
			return(0);
		if (sp->subsamplingcorrect==0)
			sp->sof_c[q]=o;
		 
		if (OJPEGReadByte(sp,&o)==0)
			return(0);
		if (sp->subsamplingcorrect!=0)
		{
			if (q==0)
			{
				sp->subsampling_hor=(o>>4);
				sp->subsampling_ver=(o&15);
				if (((sp->subsampling_hor!=1) && (sp->subsampling_hor!=2) && (sp->subsampling_hor!=4)) ||
					((sp->subsampling_ver!=1) && (sp->subsampling_ver!=2) && (sp->subsampling_ver!=4)))
					sp->subsampling_force_desubsampling_inside_decompression=1;
			}
			else
			{
				if (o!=17)
					sp->subsampling_force_desubsampling_inside_decompression=1;
			}
		}
		else
		{
			sp->sof_hv[q]=o;
			if (sp->subsampling_force_desubsampling_inside_decompression==0)
			{
				if (q==0)
				{
					if (o!=((sp->subsampling_hor<<4)|sp->subsampling_ver))
					{
						TIFFErrorExt(tif->tif_clientdata,module,"JPEG compressed data indicates unexpected subsampling values");
						return(0);
					}
				}
				else
				{
					if (o!=17)
					{
						TIFFErrorExt(tif->tif_clientdata,module,"JPEG compressed data indicates unexpected subsampling values");
						return(0);
					}
				}
			}
		}
		 
		if (OJPEGReadByte(sp,&o)==0)
			return(0);
		if (sp->subsamplingcorrect==0)
			sp->sof_tq[q]=o;
	}
	if (sp->subsamplingcorrect==0)
		sp->sof_log=1;
	return(1);
}
