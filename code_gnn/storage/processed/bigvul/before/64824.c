IW_IMPL(int) iw_write_jpeg_file(struct iw_context *ctx,  struct iw_iodescr *iodescr)
{
	int retval=0;
	struct jpeg_compress_struct cinfo;
	struct my_error_mgr jerr;
	J_COLOR_SPACE in_colortype;  
	int jpeg_cmpts;
	int compress_created = 0;
	int compress_started = 0;
	JSAMPROW *row_pointers = NULL;
	int is_grayscale;
	int j;
	struct iw_image img;
	int jpeg_quality;
	int samp_factor_h, samp_factor_v;
	int disable_subsampling = 0;
	struct iwjpegwcontext wctx;
	const char *optv;
	int ret;

	iw_zeromem(&cinfo,sizeof(struct jpeg_compress_struct));
	iw_zeromem(&jerr,sizeof(struct my_error_mgr));
	iw_zeromem(&wctx,sizeof(struct iwjpegwcontext));

	iw_get_output_image(ctx,&img);

	if(IW_IMGTYPE_HAS_ALPHA(img.imgtype)) {
		iw_set_error(ctx,"Internal: Transparency not supported with JPEG output");
		goto done;
	}

	if(img.bit_depth!=8) {
		iw_set_errorf(ctx,"Internal: Precision %d not supported with JPEG output",img.bit_depth);
		goto done;
	}

	is_grayscale = IW_IMGTYPE_IS_GRAY(img.imgtype);

	if(is_grayscale) {
		in_colortype=JCS_GRAYSCALE;
		jpeg_cmpts=1;
	}
	else {
		in_colortype=JCS_RGB;
		jpeg_cmpts=3;
	}

	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;

	if (setjmp(jerr.setjmp_buffer)) {
		char buffer[JMSG_LENGTH_MAX];

		(*cinfo.err->format_message) ((j_common_ptr)&cinfo, buffer);

		iw_set_errorf(ctx,"libjpeg reports write error: %s",buffer);

		goto done;
	}

	jpeg_create_compress(&cinfo);
	compress_created=1;

	wctx.pub.init_destination = my_init_destination_fn;
	wctx.pub.empty_output_buffer = my_empty_output_buffer_fn;
	wctx.pub.term_destination = my_term_destination_fn;
	wctx.ctx = ctx;
	wctx.iodescr = iodescr;
	wctx.buffer_len = 32768;
	wctx.buffer = iw_malloc(ctx,wctx.buffer_len);
	if(!wctx.buffer) goto done;
	cinfo.dest = (struct jpeg_destination_mgr*)&wctx;

	cinfo.image_width = img.width;
	cinfo.image_height = img.height;
	cinfo.input_components = jpeg_cmpts;
	cinfo.in_color_space = in_colortype;

	jpeg_set_defaults(&cinfo);

	optv = iw_get_option(ctx, "jpeg:block");
	if(optv) {
#if (JPEG_LIB_VERSION_MAJOR>=9 || \
	(JPEG_LIB_VERSION_MAJOR==8 && JPEG_LIB_VERSION_MINOR>=3))
		cinfo.block_size = iw_parse_int(optv);
#else
		iw_warning(ctx, "Setting block size is not supported by this version of libjpeg");
#endif
	}

	optv = iw_get_option(ctx, "jpeg:arith");
	if(optv)
		cinfo.arith_code = iw_parse_int(optv) ? TRUE : FALSE;
	else
		cinfo.arith_code = FALSE;

	optv = iw_get_option(ctx, "jpeg:colortype");
	if(optv) {
		if(!strcmp(optv, "rgb")) {
			if(in_colortype==JCS_RGB) {
				jpeg_set_colorspace(&cinfo,JCS_RGB);
				disable_subsampling = 1;
			}
		}
		else if(!strcmp(optv, "rgb1")) {
			if(in_colortype==JCS_RGB) {
#if JPEG_LIB_VERSION_MAJOR >= 9
				cinfo.color_transform = JCT_SUBTRACT_GREEN;
#else
				iw_warning(ctx, "Color type rgb1 is not supported by this version of libjpeg");
#endif
				jpeg_set_colorspace(&cinfo,JCS_RGB);
				disable_subsampling = 1;
			}
		}
	}

	optv = iw_get_option(ctx, "jpeg:bgycc");
	if(optv && iw_parse_int(optv)) {
#if (JPEG_LIB_VERSION_MAJOR>9 || \
	(JPEG_LIB_VERSION_MAJOR==9 && JPEG_LIB_VERSION_MINOR>=1))
		jpeg_set_colorspace(&cinfo, JCS_BG_YCC);
#else
		iw_warning(ctx, "Big gamut YCC is not supported by this version of libjpeg");
#endif
	}

	iwjpg_set_density(ctx,&cinfo,&img);

	optv = iw_get_option(ctx, "jpeg:quality");
	if(optv)
		jpeg_quality = iw_parse_int(optv);
	else
		jpeg_quality = 0;

	if(jpeg_quality>0) {
		jpeg_set_quality(&cinfo,jpeg_quality,0);
	}

	if(jpeg_cmpts>1 && !disable_subsampling) {
		samp_factor_h = 0;
		samp_factor_v = 0;

		optv = iw_get_option(ctx, "jpeg:sampling-x");
		if(optv)
			samp_factor_h = iw_parse_int(optv);
		optv = iw_get_option(ctx, "jpeg:sampling-y");
		if(optv)
			samp_factor_v = iw_parse_int(optv);

		optv = iw_get_option(ctx, "jpeg:sampling");
		if(optv) {
			double tmpsamp[2];
			tmpsamp[0] = 1.0;
			tmpsamp[1] = 1.0;
			ret = iw_parse_number_list(optv, 2, tmpsamp);
			samp_factor_h = iw_round_to_int(tmpsamp[0]);
			if(ret==1) {
				samp_factor_v = samp_factor_h;
			}
			else {
				samp_factor_v = iw_round_to_int(tmpsamp[1]);
			}
		}

		if(samp_factor_h>0) {
			if(samp_factor_h>4) samp_factor_h=4;
			cinfo.comp_info[0].h_samp_factor = samp_factor_h;
		}
		if(samp_factor_v>0) {
			if(samp_factor_v>4) samp_factor_v=4;
			cinfo.comp_info[0].v_samp_factor = samp_factor_v;
		}
	}

	if(iw_get_value(ctx,IW_VAL_OUTPUT_INTERLACED)) {
		jpeg_simple_progression(&cinfo);
	}

	row_pointers = (JSAMPROW*)iw_malloc(ctx, img.height * sizeof(JSAMPROW));
	if(!row_pointers) goto done;

	for(j=0;j<img.height;j++) {
		row_pointers[j] = &img.pixels[j*img.bpr];
	}

	jpeg_start_compress(&cinfo, TRUE);
	compress_started=1;

	jpeg_write_scanlines(&cinfo, row_pointers, img.height);

	retval=1;

done:
	if(compress_started)
		jpeg_finish_compress(&cinfo);

	if(compress_created)
		jpeg_destroy_compress(&cinfo);

	if(row_pointers) iw_free(ctx,row_pointers);

	if(wctx.buffer) iw_free(ctx,wctx.buffer);

	return retval;
}