static int iwbmp_read_info_header(struct iwbmprcontext *rctx)
{
	iw_byte buf[124];
	int retval = 0;
	size_t n;

	if(!iwbmp_read(rctx,buf,4)) goto done;
	rctx->infoheader_size = iw_get_ui32le(&buf[0]);
	if(rctx->infoheader_size<12) goto done;

	n = rctx->infoheader_size;
	if(n>sizeof(buf)) n=sizeof(buf);
	if(!iwbmp_read(rctx,&buf[4],n-4)) goto done;

	if(rctx->infoheader_size==12) {
		rctx->bmpversion=2;
		if(!decode_v2_header(rctx,buf)) goto done;
	}
	else if(rctx->infoheader_size==16 || rctx->infoheader_size==40 || rctx->infoheader_size==64) {
		rctx->bmpversion=3;
		if(!decode_v3_header_fields(rctx,buf)) goto done;
	}
	else if(rctx->infoheader_size==108 || rctx->infoheader_size==52 || rctx->infoheader_size==56) {
		rctx->bmpversion=4;
		if(!decode_v3_header_fields(rctx,buf)) goto done;
		if(!decode_v4_header_fields(rctx,buf)) goto done;
	}
	else if(rctx->infoheader_size==124) {
		rctx->bmpversion=5;
		if(!decode_v3_header_fields(rctx,buf)) goto done;
		if(!decode_v4_header_fields(rctx,buf)) goto done;
		if(!decode_v5_header_fields(rctx,buf)) goto done;
	}
	else {
		iw_set_error(rctx->ctx,"Unsupported BMP version");
		goto done;
	}

	if(!iw_check_image_dimensions(rctx->ctx,rctx->width,rctx->height)) {
		goto done;
	}

	retval = 1;

done:
	return retval;
}
