static int iwbmp_write_bmp_v3header(struct iwbmpwcontext *wctx)
{
	unsigned int dens_x, dens_y;
	unsigned int cmpr;
	iw_byte header[40];

	iw_zeromem(header,sizeof(header));

	iw_set_ui32le(&header[ 0],(unsigned int)wctx->header_size);  
	iw_set_ui32le(&header[ 4],wctx->img->width);   
	iw_set_ui32le(&header[ 8],wctx->img->height);  
	iw_set_ui16le(&header[12],1);     
	iw_set_ui16le(&header[14],wctx->bitcount);    

	cmpr = IWBMP_BI_RGB;
	if(wctx->compressed) {
		if(wctx->bitcount==8) cmpr = IWBMP_BI_RLE8;
		else if(wctx->bitcount==4) cmpr = IWBMP_BI_RLE4;
	}
	else if(wctx->uses_bitfields) {
		cmpr = IWBMP_BI_BITFIELDS;
	}
	iw_set_ui32le(&header[16],cmpr);  

	iw_set_ui32le(&header[20],(unsigned int)wctx->unc_bitssize);  

	if(wctx->img->density_code==IW_DENSITY_UNITS_PER_METER) {
		dens_x = (unsigned int)(0.5+wctx->img->density_x);
		dens_y = (unsigned int)(0.5+wctx->img->density_y);
	}
	else {
		dens_x = dens_y = 2835;
	}
	iw_set_ui32le(&header[24],dens_x);  
	iw_set_ui32le(&header[28],dens_y);  

	iw_set_ui32le(&header[32],wctx->palentries);     
	iwbmp_write(wctx,header,40);
	return 1;
}