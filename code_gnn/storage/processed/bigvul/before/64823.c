IW_IMPL(int) iw_read_jpeg_file(struct iw_context *ctx, struct iw_iodescr *iodescr)
{
	int retval=0;
	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
	int cinfo_valid=0;
	int colorspace;
	JDIMENSION rownum;
	JSAMPLE *jsamprow;
	int numchannels=0;
	struct iw_image img;
	struct iwjpegrcontext rctx;
	JSAMPLE *tmprow = NULL;
	int cmyk_flag = 0;

	iw_zeromem(&img,sizeof(struct iw_image));
	iw_zeromem(&cinfo,sizeof(struct jpeg_decompress_struct));
	iw_zeromem(&jerr,sizeof(struct my_error_mgr));
	iw_zeromem(&rctx,sizeof(struct iwjpegrcontext));

	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	jerr.pub.output_message = my_output_message;

	if (setjmp(jerr.setjmp_buffer)) {
		char buffer[JMSG_LENGTH_MAX];

		(*cinfo.err->format_message) ((j_common_ptr)&cinfo, buffer);

		iw_set_errorf(ctx,"libjpeg reports read error: %s",buffer);

		goto done;
	}

	jpeg_create_decompress(&cinfo);
	cinfo_valid=1;

	rctx.pub.init_source = my_init_source_fn;
	rctx.pub.fill_input_buffer = my_fill_input_buffer_fn;
	rctx.pub.skip_input_data = my_skip_input_data_fn;
	rctx.pub.resync_to_restart = jpeg_resync_to_restart;  
	rctx.pub.term_source = my_term_source_fn;
	rctx.ctx = ctx;
	rctx.iodescr = iodescr;
	rctx.buffer_len = 32768;
	rctx.buffer = iw_malloc(ctx, rctx.buffer_len);
	if(!rctx.buffer) goto done;
	rctx.exif_density_x = -1.0;
	rctx.exif_density_y = -1.0;
	cinfo.src = (struct jpeg_source_mgr*)&rctx;

	jpeg_save_markers(&cinfo, 0xe1, 65535);

	jpeg_read_header(&cinfo, TRUE);

	rctx.is_jfif = cinfo.saw_JFIF_marker;

	iwjpeg_read_density(ctx,&img,&cinfo);

	iwjpeg_read_saved_markers(&rctx,&cinfo);

	jpeg_start_decompress(&cinfo);

	colorspace=cinfo.out_color_space;
	numchannels=cinfo.output_components;


	if(colorspace==JCS_GRAYSCALE && numchannels==1) {
		img.imgtype = IW_IMGTYPE_GRAY;
		img.native_grayscale = 1;
	}
	else if((colorspace==JCS_RGB) && numchannels==3) {
		img.imgtype = IW_IMGTYPE_RGB;
	}
	else if((colorspace==JCS_CMYK) && numchannels==4) {
		img.imgtype = IW_IMGTYPE_RGB;
		cmyk_flag = 1;
	}
	else {
		iw_set_error(ctx,"Unsupported type of JPEG");
		goto done;
	}

	img.width = cinfo.output_width;
	img.height = cinfo.output_height;
	if(!iw_check_image_dimensions(ctx,img.width,img.height)) {
		goto done;
	}

	img.bit_depth = 8;
	img.bpr = iw_calc_bytesperrow(img.width,img.bit_depth*numchannels);

	img.pixels = (iw_byte*)iw_malloc_large(ctx, img.bpr, img.height);
	if(!img.pixels) {
		goto done;
	}

	if(cmyk_flag) {
		tmprow = iw_malloc(ctx,4*img.width);
		if(!tmprow) goto done;
	}

	while(cinfo.output_scanline < cinfo.output_height) {
		rownum=cinfo.output_scanline;
		jsamprow = &img.pixels[img.bpr * rownum];
		if(cmyk_flag) {
			jpeg_read_scanlines(&cinfo, &tmprow, 1);
			convert_cmyk_to_rbg(ctx,tmprow,jsamprow,img.width);
		}
		else {
			jpeg_read_scanlines(&cinfo, &jsamprow, 1);
		}
		if(cinfo.output_scanline<=rownum) {
			iw_set_error(ctx,"Error reading JPEG file");
			goto done;
		}
	}
	jpeg_finish_decompress(&cinfo);

	handle_exif_density(&rctx, &img);

	iw_set_input_image(ctx, &img);
	img.pixels = NULL;

	if(rctx.exif_orientation>=2 && rctx.exif_orientation<=8) {
		static const unsigned int exif_orient_to_transform[9] =
		   { 0,0, 1,3,2,4,5,7,6 };


		if(rctx.is_jfif) {
			iw_warning(ctx,"JPEG image has an ambiguous orientation");
		}
		iw_reorient_image(ctx,exif_orient_to_transform[rctx.exif_orientation]);
	}

	retval=1;

done:
	iw_free(ctx, img.pixels);
	if(cinfo_valid) jpeg_destroy_decompress(&cinfo);
	if(rctx.buffer) iw_free(ctx,rctx.buffer);
	if(tmprow) iw_free(ctx,tmprow);
	return retval;
}