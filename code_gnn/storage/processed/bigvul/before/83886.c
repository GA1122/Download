vips__foreign_convert_saveable( VipsImage *in, VipsImage **ready,
	VipsSaveable saveable, VipsBandFormat *format, VipsCoding *coding,
	VipsArrayDouble *background )
{
	 
	g_object_ref( in );

	 
	if( in->Coding != VIPS_CODING_NONE &&
		coding[in->Coding] ) {
		*ready = in;
		return( 0 );
	}

	 
	if( in->Coding == VIPS_CODING_NONE &&
	        saveable == VIPS_SAVEABLE_ANY &&
		format[in->BandFmt] == in->BandFmt ) {
		*ready = in;
		return( 0 );
	}

	 

	 
	if( in->Coding == VIPS_CODING_LABQ ) {
		VipsImage *out;

		if( vips_LabQ2sRGB( in, &out, NULL ) ) {
			g_object_unref( in );
			return( -1 );
		}
		g_object_unref( in );

		in = out;
	}

	 
	if( in->Coding == VIPS_CODING_RAD ) {
		VipsImage *out;

		if( vips_rad2float( in, &out, NULL ) ) {
			g_object_unref( in );
			return( -1 );
		}
		g_object_unref( in );

		in = out;
	}

	 
	if( coding[VIPS_CODING_RAD] ) {
		if( in->Type != VIPS_INTERPRETATION_scRGB &&
			in->Type != VIPS_INTERPRETATION_XYZ ) {
			VipsImage *out;

			if( vips_colourspace( in, &out, 
				VIPS_INTERPRETATION_scRGB, NULL ) ) {
				g_object_unref( in );
				return( -1 );
			}
			g_object_unref( in );

			in = out;
		}
	}

	 
	if( in->Type == VIPS_INTERPRETATION_CMYK &&
		in->Bands >= 4 &&
		(saveable == VIPS_SAVEABLE_RGB ||
		 saveable == VIPS_SAVEABLE_RGBA ||
		 saveable == VIPS_SAVEABLE_RGBA_ONLY) ) { 
		VipsImage *out;

		if( vips_icc_import( in, &out, 
			"pcs", VIPS_PCS_XYZ,
			NULL ) ) {
			g_object_unref( in );
			return( -1 );
		}
		g_object_unref( in );

		in = out;

		 
		vips_image_remove( in, VIPS_META_ICC_NAME );
	}

	 
	if( !coding[VIPS_CODING_RAD] &&
		in->Bands >= 3 &&
		in->Type != VIPS_INTERPRETATION_CMYK &&
		vips_colourspace_issupported( in ) &&
		(saveable == VIPS_SAVEABLE_RGB ||
		 saveable == VIPS_SAVEABLE_RGBA ||
		 saveable == VIPS_SAVEABLE_RGBA_ONLY ||
		 saveable == VIPS_SAVEABLE_RGB_CMYK) ) { 
		VipsImage *out;
		VipsInterpretation interpretation;

		 
		if( vips_band_format_is8bit( format[in->BandFmt] ) )
			interpretation = VIPS_INTERPRETATION_sRGB;
		else
			interpretation = VIPS_INTERPRETATION_RGB16;

		if( vips_colourspace( in, &out, interpretation, NULL ) ) {
			g_object_unref( in );
			return( -1 );
		}
		g_object_unref( in );

		in = out;
	}

	 
	if( !coding[VIPS_CODING_RAD] &&
		in->Bands < 3 &&
		vips_colourspace_issupported( in ) &&
		saveable == VIPS_SAVEABLE_RGBA_ONLY ) { 
		VipsImage *out;
		VipsInterpretation interpretation;

		 
		if( vips_band_format_is8bit( format[in->BandFmt] ) )
			interpretation = VIPS_INTERPRETATION_sRGB;
		else
			interpretation = VIPS_INTERPRETATION_RGB16;

		if( vips_colourspace( in, &out, interpretation, NULL ) ) {
			g_object_unref( in );
			return( -1 );
		}
		g_object_unref( in );

		in = out;
	}

	 
	if( in->Coding == VIPS_CODING_NONE ) {
		 
		if( (in->Bands == 2 ||
			(in->Bands == 4 && 
			 in->Type != VIPS_INTERPRETATION_CMYK)) &&
			(saveable == VIPS_SAVEABLE_MONO ||
			 saveable == VIPS_SAVEABLE_RGB ||
			 saveable == VIPS_SAVEABLE_RGB_CMYK) ) {
			VipsImage *out;

			if( vips_flatten( in, &out, 
				"background", background,
				NULL ) ) {
				g_object_unref( in );
				return( -1 );
			}
			g_object_unref( in );

			in = out;
		}

		 

		else if( in->Bands > 3 && 
			(saveable == VIPS_SAVEABLE_RGB ||
			 (saveable == VIPS_SAVEABLE_RGB_CMYK &&
			  in->Type != VIPS_INTERPRETATION_CMYK)) ) { 
			VipsImage *out;

			 
			if( vips_extract_band( in, &out, 0, 
				"n", 3,
				NULL ) ) {
				g_object_unref( in );
				return( -1 );
			}
			g_object_unref( in );

			in = out;
		}
		else if( in->Bands > 4 && 
			((saveable == VIPS_SAVEABLE_RGB_CMYK &&
			  in->Type == VIPS_INTERPRETATION_CMYK) ||
			 saveable == VIPS_SAVEABLE_RGBA ||
			 saveable == VIPS_SAVEABLE_RGBA_ONLY) ) {
			VipsImage *out;

			if( vips_extract_band( in, &out, 0, 
				"n", 4,
				NULL ) ) {
				g_object_unref( in );
				return( -1 );
			}
			g_object_unref( in );

			in = out;
		}
		else if( in->Bands > 1 && 
			saveable == VIPS_SAVEABLE_MONO ) {
			VipsImage *out;

			if( vips_extract_band( in, &out, 0, NULL ) ) {
				g_object_unref( in );
				return( -1 );
			}
			g_object_unref( in );

			in = out;
		}

		 
	}

	 
	if( (in->Type == VIPS_INTERPRETATION_RGB16 ||
		 in->Type == VIPS_INTERPRETATION_GREY16) &&
		!vips_band_format_is8bit( in->BandFmt ) ) {
		 
		if( format[VIPS_FORMAT_USHORT] == VIPS_FORMAT_USHORT ) {
			VipsImage *out;

			if( vips_cast( in, &out, VIPS_FORMAT_USHORT, NULL ) ) {
				g_object_unref( in );
				return( -1 );
			}
			g_object_unref( in );

			in = out;
		}
		else {
			VipsImage *out;

			if( vips_rshift_const1( in, &out, 8, NULL ) ) { 
				g_object_unref( in );
				return( -1 );
			}
			g_object_unref( in );

			in = out;

			 
			if( vips_cast( in, &out, VIPS_FORMAT_UCHAR, NULL ) ) {
				g_object_unref( in );
				return( -1 );
			}
			g_object_unref( in );

			in = out;
		}
	}

	 
	{
		VipsImage *out;

		if( vips_cast( in, &out, format[in->BandFmt], NULL ) ) {
			g_object_unref( in );
			return( -1 );
		}
		g_object_unref( in );

		in = out;
	}

	 
	if( coding[VIPS_CODING_NONE] ) {
		 
	}
	else if( coding[VIPS_CODING_LABQ] ) {
		VipsImage *out;

		if( vips_Lab2LabQ( in, &out, NULL ) ) {
			g_object_unref( in );
			return( -1 );
		}
		g_object_unref( in );

		in = out;
	}
	else if( coding[VIPS_CODING_RAD] ) {
		VipsImage *out;

		if( vips_float2rad( in, &out, NULL ) ) {
			g_object_unref( in );
			return( -1 );
		}
		g_object_unref( in );

		in = out;
	}

	*ready = in;

	return( 0 );
}
