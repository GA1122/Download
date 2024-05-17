pdf_show_image(fz_context *ctx, pdf_run_processor *pr, fz_image *image)
{
	pdf_gstate *gstate = pr->gstate + pr->gtop;
	fz_matrix image_ctm;
	fz_rect bbox;
	softmask_save softmask = { NULL };

	if (pr->super.hidden)
		return;

	 
	image_ctm = gstate->ctm;
	fz_pre_scale(fz_pre_translate(&image_ctm, 0, 1), 1, -1);

	bbox = fz_unit_rect;
	fz_transform_rect(&bbox, &image_ctm);

	if (image->mask)
	{
		 
		if (gstate->blendmode)
			fz_begin_group(ctx, pr->dev, &bbox, NULL, 0, 0, gstate->blendmode, 1);
		fz_clip_image_mask(ctx, pr->dev, image->mask, &image_ctm, &bbox);
	}
	else
		gstate = pdf_begin_group(ctx, pr, &bbox, &softmask);

	if (!image->colorspace)
	{
		switch (gstate->fill.kind)
		{
		case PDF_MAT_NONE:
			break;
		case PDF_MAT_COLOR:
			fz_fill_image_mask(ctx, pr->dev, image, &image_ctm,
				gstate->fill.colorspace, gstate->fill.v, gstate->fill.alpha, &gstate->fill.color_params);
			break;
		case PDF_MAT_PATTERN:
			if (gstate->fill.pattern)
			{
				fz_clip_image_mask(ctx, pr->dev, image, &image_ctm, &bbox);
				pdf_show_pattern(ctx, pr, gstate->fill.pattern, &pr->gstate[gstate->fill.gstate_num], &bbox, PDF_FILL);
				fz_pop_clip(ctx, pr->dev);
			}
			break;
		case PDF_MAT_SHADE:
			if (gstate->fill.shade)
			{
				fz_clip_image_mask(ctx, pr->dev, image, &image_ctm, &bbox);
				fz_fill_shade(ctx, pr->dev, gstate->fill.shade, &pr->gstate[gstate->fill.gstate_num].ctm, gstate->fill.alpha, &gstate->fill.color_params);
				fz_pop_clip(ctx, pr->dev);
			}
			break;
		}
	}
	else
	{
		fz_fill_image(ctx, pr->dev, image, &image_ctm, gstate->fill.alpha, &gstate->fill.color_params);
	}

	if (image->mask)
	{
		fz_pop_clip(ctx, pr->dev);
		if (gstate->blendmode)
			fz_end_group(ctx, pr->dev);
	}
	else
		pdf_end_group(ctx, pr, &softmask);
}
