static int iwbmp_read_file_header(struct iwbmprcontext *rctx)
{
	iw_byte buf[14];

	if(!iwbmp_read(rctx,buf,14)) return 0;
	rctx->fileheader_size = 14;

	if(buf[0]=='B' && buf[1]=='A') {  
		if(!iwbmp_read(rctx,buf,14)) return 0;
		rctx->fileheader_size += 14;
	}

	if(buf[0]=='B' && buf[1]=='M') {
		;
	}
	else if((buf[0]=='C' && buf[1]=='I') ||  
	   (buf[0]=='C' && buf[1]=='P') ||  
	   (buf[0]=='I' && buf[1]=='C') ||  
	   (buf[0]=='P' && buf[1]=='T'))    
	{
		iw_set_error(rctx->ctx,"This type of BMP file is not supported");
		return 0;
	}
	else {
		iw_set_error(rctx->ctx,"Not a BMP file");
		return 0;
	}

	rctx->bfOffBits = iw_get_ui32le(&buf[10]);
	return 1;
}
