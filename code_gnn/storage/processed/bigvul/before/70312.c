OJPEGWriteHeaderInfo(TIFF* tif)
{
	static const char module[]="OJPEGWriteHeaderInfo";
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint8** m;
	uint32 n;
	 
	if (sp->libjpeg_session_active != 0) 
		return 0;
	sp->out_state=ososSoi;
	sp->restart_index=0;
	jpeg_std_error(&(sp->libjpeg_jpeg_error_mgr));
	sp->libjpeg_jpeg_error_mgr.output_message=OJPEGLibjpegJpegErrorMgrOutputMessage;
	sp->libjpeg_jpeg_error_mgr.error_exit=OJPEGLibjpegJpegErrorMgrErrorExit;
	sp->libjpeg_jpeg_decompress_struct.err=&(sp->libjpeg_jpeg_error_mgr);
	sp->libjpeg_jpeg_decompress_struct.client_data=(void*)tif;
	if (jpeg_create_decompress_encap(sp,&(sp->libjpeg_jpeg_decompress_struct))==0)
		return(0);
	sp->libjpeg_session_active=1;
	sp->libjpeg_jpeg_source_mgr.bytes_in_buffer=0;
	sp->libjpeg_jpeg_source_mgr.init_source=OJPEGLibjpegJpegSourceMgrInitSource;
	sp->libjpeg_jpeg_source_mgr.fill_input_buffer=OJPEGLibjpegJpegSourceMgrFillInputBuffer;
	sp->libjpeg_jpeg_source_mgr.skip_input_data=OJPEGLibjpegJpegSourceMgrSkipInputData;
	sp->libjpeg_jpeg_source_mgr.resync_to_restart=OJPEGLibjpegJpegSourceMgrResyncToRestart;
	sp->libjpeg_jpeg_source_mgr.term_source=OJPEGLibjpegJpegSourceMgrTermSource;
	sp->libjpeg_jpeg_decompress_struct.src=&(sp->libjpeg_jpeg_source_mgr);
	if (jpeg_read_header_encap(sp,&(sp->libjpeg_jpeg_decompress_struct),1)==0)
		return(0);
	if ((sp->subsampling_force_desubsampling_inside_decompression==0) && (sp->samples_per_pixel_per_plane>1))
	{
		sp->libjpeg_jpeg_decompress_struct.raw_data_out=1;
#if JPEG_LIB_VERSION >= 70
		sp->libjpeg_jpeg_decompress_struct.do_fancy_upsampling=FALSE;
#endif
		sp->libjpeg_jpeg_query_style=0;
		if (sp->subsampling_convert_log==0)
		{
			assert(sp->subsampling_convert_ycbcrbuf==0);
			assert(sp->subsampling_convert_ycbcrimage==0);
			sp->subsampling_convert_ylinelen=((sp->strile_width+sp->subsampling_hor*8-1)/(sp->subsampling_hor*8)*sp->subsampling_hor*8);
			sp->subsampling_convert_ylines=sp->subsampling_ver*8;
			sp->subsampling_convert_clinelen=sp->subsampling_convert_ylinelen/sp->subsampling_hor;
			sp->subsampling_convert_clines=8;
			sp->subsampling_convert_ybuflen=sp->subsampling_convert_ylinelen*sp->subsampling_convert_ylines;
			sp->subsampling_convert_cbuflen=sp->subsampling_convert_clinelen*sp->subsampling_convert_clines;
			sp->subsampling_convert_ycbcrbuflen=sp->subsampling_convert_ybuflen+2*sp->subsampling_convert_cbuflen;
			sp->subsampling_convert_ycbcrbuf=_TIFFmalloc(sp->subsampling_convert_ycbcrbuflen);
			if (sp->subsampling_convert_ycbcrbuf==0)
			{
				TIFFErrorExt(tif->tif_clientdata,module,"Out of memory");
				return(0);
			}
			sp->subsampling_convert_ybuf=sp->subsampling_convert_ycbcrbuf;
			sp->subsampling_convert_cbbuf=sp->subsampling_convert_ybuf+sp->subsampling_convert_ybuflen;
			sp->subsampling_convert_crbuf=sp->subsampling_convert_cbbuf+sp->subsampling_convert_cbuflen;
			sp->subsampling_convert_ycbcrimagelen=3+sp->subsampling_convert_ylines+2*sp->subsampling_convert_clines;
			sp->subsampling_convert_ycbcrimage=_TIFFmalloc(sp->subsampling_convert_ycbcrimagelen*sizeof(uint8*));
			if (sp->subsampling_convert_ycbcrimage==0)
			{
				TIFFErrorExt(tif->tif_clientdata,module,"Out of memory");
				return(0);
			}
			m=sp->subsampling_convert_ycbcrimage;
			*m++=(uint8*)(sp->subsampling_convert_ycbcrimage+3);
			*m++=(uint8*)(sp->subsampling_convert_ycbcrimage+3+sp->subsampling_convert_ylines);
			*m++=(uint8*)(sp->subsampling_convert_ycbcrimage+3+sp->subsampling_convert_ylines+sp->subsampling_convert_clines);
			for (n=0; n<sp->subsampling_convert_ylines; n++)
				*m++=sp->subsampling_convert_ybuf+n*sp->subsampling_convert_ylinelen;
			for (n=0; n<sp->subsampling_convert_clines; n++)
				*m++=sp->subsampling_convert_cbbuf+n*sp->subsampling_convert_clinelen;
			for (n=0; n<sp->subsampling_convert_clines; n++)
				*m++=sp->subsampling_convert_crbuf+n*sp->subsampling_convert_clinelen;
			sp->subsampling_convert_clinelenout=((sp->strile_width+sp->subsampling_hor-1)/sp->subsampling_hor);
			sp->subsampling_convert_state=0;
			sp->bytes_per_line=sp->subsampling_convert_clinelenout*(sp->subsampling_ver*sp->subsampling_hor+2);
			sp->lines_per_strile=((sp->strile_length+sp->subsampling_ver-1)/sp->subsampling_ver);
			sp->subsampling_convert_log=1;
		}
	}
	else
	{
		sp->libjpeg_jpeg_decompress_struct.jpeg_color_space=JCS_UNKNOWN;
		sp->libjpeg_jpeg_decompress_struct.out_color_space=JCS_UNKNOWN;
		sp->libjpeg_jpeg_query_style=1;
		sp->bytes_per_line=sp->samples_per_pixel_per_plane*sp->strile_width;
		sp->lines_per_strile=sp->strile_length;
	}
	if (jpeg_start_decompress_encap(sp,&(sp->libjpeg_jpeg_decompress_struct))==0)
		return(0);
	sp->writeheader_done=1;
	return(1);
}