jas_image_t *jas_image_decode(jas_stream_t *in, int fmt, char *optstr)
{
	jas_image_fmtinfo_t *fmtinfo;
	jas_image_t *image;

	image = 0;

	 
	if (fmt < 0) {
		if ((fmt = jas_image_getfmt(in)) < 0)
			goto error;
	}

	 
	if (!(fmtinfo = jas_image_lookupfmtbyid(fmt)))
		goto error;
	if (!fmtinfo->ops.decode)
		goto error;

	 
	if (!(image = (*fmtinfo->ops.decode)(in, optstr)))
		goto error;

	 
	if (!jas_clrspc_isunknown(image->clrspc_) &&
	  !jas_clrspc_isgeneric(image->clrspc_) && !image->cmprof_) {
		if (!(image->cmprof_ =
		  jas_cmprof_createfromclrspc(jas_image_clrspc(image))))
			goto error;
	}

	return image;
error:
	if (image)
		jas_image_destroy(image);
	return 0;
}
