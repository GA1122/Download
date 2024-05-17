static void iw_make_nearest_color_table(struct iw_context *ctx, double **ptable,
	const struct iw_image *img, const struct iw_csdescr *csdescr)
{
	int ncolors;
	int nentries;
	int i;
	double *tbl;
	double prev;
	double curr;

	if(ctx->no_gamma) return;
	if(csdescr->cstype==IW_CSTYPE_LINEAR) return;
	if(img->sampletype==IW_SAMPLETYPE_FLOATINGPOINT) return;
	if(img->bit_depth != ctx->img2.bit_depth) return;

	ncolors = (1 << img->bit_depth);
	if(ncolors>256) return;
	nentries = ncolors-1;

	if( ((size_t)img->width)*img->height <= 512 ) return;

	tbl = iw_malloc(ctx,nentries*sizeof(double));
	if(!tbl) return;

	prev = 0.0;
	for(i=0;i<nentries;i++) {
		curr = x_to_linear_sample( ((double)(i+1))/(ncolors-1), csdescr);
		tbl[i] = (prev + curr)/2.0;
		prev = curr;
	}

	*ptable = tbl;
}
