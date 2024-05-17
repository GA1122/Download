OJPEGReadHeaderInfoSec(TIFF* tif)
{
	static const char module[]="OJPEGReadHeaderInfoSec";
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint8 m;
	uint16 n;
	uint8 o;
	if (sp->file_size==0)
		sp->file_size=TIFFGetFileSize(tif);
	if (sp->jpeg_interchange_format!=0)
	{
		if (sp->jpeg_interchange_format>=sp->file_size)
		{
			sp->jpeg_interchange_format=0;
			sp->jpeg_interchange_format_length=0;
		}
		else
		{
			if ((sp->jpeg_interchange_format_length==0) || (sp->jpeg_interchange_format+sp->jpeg_interchange_format_length>sp->file_size))
				sp->jpeg_interchange_format_length=sp->file_size-sp->jpeg_interchange_format;
		}
	}
	sp->in_buffer_source=osibsNotSetYet;
	sp->in_buffer_next_strile=0;
	sp->in_buffer_strile_count=tif->tif_dir.td_nstrips;
	sp->in_buffer_file_togo=0;
	sp->in_buffer_togo=0;
	do
	{
		if (OJPEGReadBytePeek(sp,&m)==0)
			return(0);
		if (m!=255)
			break;
		OJPEGReadByteAdvance(sp);
		do
		{
			if (OJPEGReadByte(sp,&m)==0)
				return(0);
		} while(m==255);
		switch(m)
		{
			case JPEG_MARKER_SOI:
				 
				break;
			case JPEG_MARKER_COM:
			case JPEG_MARKER_APP0:
			case JPEG_MARKER_APP0+1:
			case JPEG_MARKER_APP0+2:
			case JPEG_MARKER_APP0+3:
			case JPEG_MARKER_APP0+4:
			case JPEG_MARKER_APP0+5:
			case JPEG_MARKER_APP0+6:
			case JPEG_MARKER_APP0+7:
			case JPEG_MARKER_APP0+8:
			case JPEG_MARKER_APP0+9:
			case JPEG_MARKER_APP0+10:
			case JPEG_MARKER_APP0+11:
			case JPEG_MARKER_APP0+12:
			case JPEG_MARKER_APP0+13:
			case JPEG_MARKER_APP0+14:
			case JPEG_MARKER_APP0+15:
				 
				if (OJPEGReadWord(sp,&n)==0)
					return(0);
				if (n<2)
				{
					if (sp->subsamplingcorrect==0)
						TIFFErrorExt(tif->tif_clientdata,module,"Corrupt JPEG data");
					return(0);
				}
				if (n>2)
					OJPEGReadSkip(sp,n-2);
				break;
			case JPEG_MARKER_DRI:
				if (OJPEGReadHeaderInfoSecStreamDri(tif)==0)
					return(0);
				break;
			case JPEG_MARKER_DQT:
				if (OJPEGReadHeaderInfoSecStreamDqt(tif)==0)
					return(0);
				break;
			case JPEG_MARKER_DHT:
				if (OJPEGReadHeaderInfoSecStreamDht(tif)==0)
					return(0);
				break;
			case JPEG_MARKER_SOF0:
			case JPEG_MARKER_SOF1:
			case JPEG_MARKER_SOF3:
				if (OJPEGReadHeaderInfoSecStreamSof(tif,m)==0)
					return(0);
				if (sp->subsamplingcorrect!=0)
					return(1);
				break;
			case JPEG_MARKER_SOS:
				if (sp->subsamplingcorrect!=0)
					return(1);
				assert(sp->plane_sample_offset==0);
				if (OJPEGReadHeaderInfoSecStreamSos(tif)==0)
					return(0);
				break;
			default:
				TIFFErrorExt(tif->tif_clientdata,module,"Unknown marker type %d in JPEG data",m);
				return(0);
		}
	} while(m!=JPEG_MARKER_SOS);
	if (sp->subsamplingcorrect)
		return(1);
	if (sp->sof_log==0)
	{
		if (OJPEGReadHeaderInfoSecTablesQTable(tif)==0)
			return(0);
		sp->sof_marker_id=JPEG_MARKER_SOF0;
		for (o=0; o<sp->samples_per_pixel; o++)
			sp->sof_c[o]=o;
		sp->sof_hv[0]=((sp->subsampling_hor<<4)|sp->subsampling_ver);
		for (o=1; o<sp->samples_per_pixel; o++)
			sp->sof_hv[o]=17;
		sp->sof_x=sp->strile_width;
		sp->sof_y=sp->strile_length_total;
		sp->sof_log=1;
		if (OJPEGReadHeaderInfoSecTablesDcTable(tif)==0)
			return(0);
		if (OJPEGReadHeaderInfoSecTablesAcTable(tif)==0)
			return(0);
		for (o=1; o<sp->samples_per_pixel; o++)
			sp->sos_cs[o]=o;
	}
	return(1);
}