OJPEGVSetField(TIFF* tif, uint32 tag, va_list ap)
{
	static const char module[]="OJPEGVSetField";
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint32 ma;
	uint64* mb;
	uint32 n;
	const TIFFField* fip;

	switch(tag)
	{
		case TIFFTAG_JPEGIFOFFSET:
			sp->jpeg_interchange_format=(uint64)va_arg(ap,uint64);
			break;
		case TIFFTAG_JPEGIFBYTECOUNT:
			sp->jpeg_interchange_format_length=(uint64)va_arg(ap,uint64);
			break;
		case TIFFTAG_YCBCRSUBSAMPLING:
			sp->subsampling_tag=1;
			sp->subsampling_hor=(uint8)va_arg(ap,uint16_vap);
			sp->subsampling_ver=(uint8)va_arg(ap,uint16_vap);
			tif->tif_dir.td_ycbcrsubsampling[0]=sp->subsampling_hor;
			tif->tif_dir.td_ycbcrsubsampling[1]=sp->subsampling_ver;
			break;
		case TIFFTAG_JPEGQTABLES:
			ma=(uint32)va_arg(ap,uint32);
			if (ma!=0)
			{
				if (ma>3)
				{
					TIFFErrorExt(tif->tif_clientdata,module,"JpegQTables tag has incorrect count");
					return(0);
				}
				sp->qtable_offset_count=(uint8)ma;
				mb=(uint64*)va_arg(ap,uint64*);
				for (n=0; n<ma; n++)
					sp->qtable_offset[n]=mb[n];
			}
			break;
		case TIFFTAG_JPEGDCTABLES:
			ma=(uint32)va_arg(ap,uint32);
			if (ma!=0)
			{
				if (ma>3)
				{
					TIFFErrorExt(tif->tif_clientdata,module,"JpegDcTables tag has incorrect count");
					return(0);
				}
				sp->dctable_offset_count=(uint8)ma;
				mb=(uint64*)va_arg(ap,uint64*);
				for (n=0; n<ma; n++)
					sp->dctable_offset[n]=mb[n];
			}
			break;
		case TIFFTAG_JPEGACTABLES:
			ma=(uint32)va_arg(ap,uint32);
			if (ma!=0)
			{
				if (ma>3)
				{
					TIFFErrorExt(tif->tif_clientdata,module,"JpegAcTables tag has incorrect count");
					return(0);
				}
				sp->actable_offset_count=(uint8)ma;
				mb=(uint64*)va_arg(ap,uint64*);
				for (n=0; n<ma; n++)
					sp->actable_offset[n]=mb[n];
			}
			break;
		case TIFFTAG_JPEGPROC:
			sp->jpeg_proc=(uint8)va_arg(ap,uint16_vap);
			break;
		case TIFFTAG_JPEGRESTARTINTERVAL:
			sp->restart_interval=(uint16)va_arg(ap,uint16_vap);
			break;
		default:
			return (*sp->vsetparent)(tif,tag,ap);
	}
	fip = TIFFFieldWithTag(tif,tag);
	if( fip == NULL )  
	    return(0);
	TIFFSetFieldBit(tif,fip->field_bit);
	tif->tif_flags|=TIFF_DIRTYDIRECT;
	return(1);
}