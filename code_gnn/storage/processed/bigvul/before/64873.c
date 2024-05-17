static int iwbmp_write_bmp_v2header(struct iwbmpwcontext *wctx)
{
	iw_byte header[12];

	if(wctx->img->width>65535 || wctx->img->height>65535) {
		iw_set_error(wctx->ctx,"Output image is too large for this BMP version");
		return 0;
	}

	iw_zeromem(header,sizeof(header));
	iw_set_ui32le(&header[ 0],12);                 
	iw_set_ui16le(&header[ 4],wctx->img->width);   
	iw_set_ui16le(&header[ 6],wctx->img->height);  
	iw_set_ui16le(&header[ 8],1);                  
	iw_set_ui16le(&header[10],wctx->bitcount);     

	iwbmp_write(wctx,header,12);
	return 1;
}