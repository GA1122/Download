gdImagePtr gdImageCreateFromJpegCtxEx (gdIOCtx * infile, int ignore_warning)
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	jmpbuf_wrapper jmpbufw;
	 
	volatile JSAMPROW row = 0;
	volatile gdImagePtr im = 0;
	JSAMPROW rowptr[1];
	unsigned int i, j;
	int retval;
	JDIMENSION nrows;
	int channels = 3;
	int inverted = 0;

	memset (&cinfo, 0, sizeof (cinfo));
	memset (&jerr, 0, sizeof (jerr));

	jmpbufw.ignore_warning = ignore_warning;

	cinfo.err = jpeg_std_error (&jerr);
	cinfo.client_data = &jmpbufw;

	cinfo.err->emit_message = (void (*)(j_common_ptr,int)) php_jpeg_emit_message;

	if (setjmp (jmpbufw.jmpbuf) != 0) {
		 
		if (row) {
			gdFree (row);
		}
		if (im) {
			gdImageDestroy (im);
		}
		return 0;
	}

	cinfo.err->error_exit = fatal_jpeg_error;

	jpeg_create_decompress (&cinfo);

	jpeg_gdIOCtx_src (&cinfo, infile);

	 
	jpeg_save_markers(&cinfo, JPEG_APP0 + 14, 256);

	retval = jpeg_read_header (&cinfo, TRUE);
	if (retval != JPEG_HEADER_OK) {
		gd_error_ex(GD_WARNING, "gd-jpeg: warning: jpeg_read_header returned %d, expected %d", retval, JPEG_HEADER_OK);
	}

	if (cinfo.image_height > INT_MAX) {
		gd_error_ex(GD_WARNING, "gd-jpeg: warning: JPEG image height (%u) is greater than INT_MAX (%d) (and thus greater than gd can handle)", cinfo.image_height, INT_MAX);
	}

	if (cinfo.image_width > INT_MAX) {
		gd_error_ex(GD_WARNING, "gd-jpeg: warning: JPEG image width (%u) is greater than INT_MAX (%d) (and thus greater than gd can handle)", cinfo.image_width, INT_MAX);
	}

	im = gdImageCreateTrueColor ((int) cinfo.image_width, (int) cinfo.image_height);
	if (im == 0) {
		gd_error("gd-jpeg error: cannot allocate gdImage struct");
		goto error;
	}

	 
	switch (cinfo.density_unit) {
	case 1:
		im->res_x = cinfo.X_density;
		im->res_y = cinfo.Y_density;
		break;
	case 2:
		im->res_x = DPCM2DPI(cinfo.X_density);
		im->res_y = DPCM2DPI(cinfo.Y_density);
		break;
	}

	 
	if ((cinfo.jpeg_color_space == JCS_CMYK) || (cinfo.jpeg_color_space == JCS_YCCK)) {
		cinfo.out_color_space = JCS_CMYK;
	} else {
		cinfo.out_color_space = JCS_RGB;
	}

	if (jpeg_start_decompress (&cinfo) != TRUE) {
		gd_error("gd-jpeg: warning: jpeg_start_decompress reports suspended data source");
	}

	 
#if 0
  gdImageInterlace (im, cinfo.progressive_mode != 0);
#endif

	if (cinfo.out_color_space == JCS_RGB) {
		if (cinfo.output_components != 3) {
			gd_error_ex(GD_WARNING, "gd-jpeg: error: JPEG color quantization request resulted in output_components == %d (expected 3 for RGB)", cinfo.output_components);
			goto error;
		}
		channels = 3;
	} else if (cinfo.out_color_space == JCS_CMYK) {
		jpeg_saved_marker_ptr marker;
		if (cinfo.output_components != 4)  {
			gd_error_ex(GD_WARNING, "gd-jpeg: error: JPEG color quantization request resulted in output_components == %d (expected 4 for CMYK)", cinfo.output_components);
			goto error;
		}
		channels = 4;
		marker = cinfo.marker_list;
		while (marker) {
			if ((marker->marker == (JPEG_APP0 + 14)) && (marker->data_length >= 12) && (!strncmp((const char *) marker->data, "Adobe", 5))) {
				inverted = 1;
				break;
			}
			marker = marker->next;
		}
	} else {
		gd_error_ex(GD_WARNING, "gd-jpeg: error: unexpected colorspace.");
		goto error;
	}

#if BITS_IN_JSAMPLE == 12
	gd_error("gd-jpeg: error: jpeg library was compiled for 12-bit precision. This is mostly useless, because JPEGs on the web are 8-bit and such versions of the jpeg library won't read or write them. GD doesn't support these unusual images. Edit your jmorecfg.h file to specify the correct precision and completely 'make clean' and 'make install' libjpeg again. Sorry.");
	goto error;
#endif  

	row = safe_emalloc(cinfo.output_width * channels, sizeof(JSAMPLE), 0);
	memset(row, 0, cinfo.output_width * channels * sizeof(JSAMPLE));
	rowptr[0] = row;

	if (cinfo.out_color_space == JCS_CMYK) {
		for (i = 0; i < cinfo.output_height; i++) {
			register JSAMPROW currow = row;
			register int *tpix = im->tpixels[i];
			nrows = jpeg_read_scanlines (&cinfo, rowptr, 1);
			if (nrows != 1) {
				gd_error_ex(GD_WARNING, "gd-jpeg: error: jpeg_read_scanlines returns %u, expected 1", nrows);
				goto error;
			}
			for (j = 0; j < cinfo.output_width; j++, currow += 4, tpix++) {
				*tpix = CMYKToRGB (currow[0], currow[1], currow[2], currow[3], inverted);
			}
		}
	} else {
		for (i = 0; i < cinfo.output_height; i++) {
			register JSAMPROW currow = row;
			register int *tpix = im->tpixels[i];
			nrows = jpeg_read_scanlines (&cinfo, rowptr, 1);
			if (nrows != 1) {
				gd_error_ex(GD_WARNING, "gd-jpeg: error: jpeg_read_scanlines returns %u, expected 1", nrows);
				goto error;
			}
			for (j = 0; j < cinfo.output_width; j++, currow += 3, tpix++) {
				*tpix = gdTrueColor (currow[0], currow[1], currow[2]);
			}
		}
	}

	if (jpeg_finish_decompress (&cinfo) != TRUE) {
		gd_error("gd-jpeg: warning: jpeg_finish_decompress reports suspended data source");
	}
	if (!ignore_warning) {
		if (cinfo.err->num_warnings > 0) {
			goto error;
		}
	}

	jpeg_destroy_decompress (&cinfo);
	gdFree (row);

	return im;

error:
	jpeg_destroy_decompress (&cinfo);
	if (row) {
		gdFree (row);
	}
	if (im) {
		gdImageDestroy (im);
	}
	return 0;
}