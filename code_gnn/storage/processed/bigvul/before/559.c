pdf_show_pattern(fz_context *ctx, pdf_run_processor *pr, pdf_pattern *pat, pdf_gstate *pat_gstate, const fz_rect *area, int what)
{
	pdf_gstate *gstate;
	int gparent_save;
	fz_matrix ptm, invptm, gparent_save_ctm;
	int x0, y0, x1, y1;
	float fx0, fy0, fx1, fy1;
	fz_rect local_area;
	int id;

	pdf_gsave(ctx, pr);
	gstate = pr->gstate + pr->gtop;

	 
	pdf_copy_pattern_gstate(ctx, gstate, pat_gstate);

	if (pat->ismask)
	{
		pdf_unset_pattern(ctx, pr, PDF_FILL);
		pdf_unset_pattern(ctx, pr, PDF_STROKE);
		if (what == PDF_FILL)
		{
			pdf_drop_material(ctx, &gstate->stroke);
			pdf_keep_material(ctx, &gstate->fill);
			gstate->stroke = gstate->fill;
		}
		if (what == PDF_STROKE)
		{
			pdf_drop_material(ctx, &gstate->fill);
			pdf_keep_material(ctx, &gstate->stroke);
			gstate->fill = gstate->stroke;
		}
		id = 0;  
	}
	else
	{
		pdf_unset_pattern(ctx, pr, what);
		id = pat->id;
	}

	 
	if (gstate->softmask)
	{
		pdf_drop_xobject(ctx, gstate->softmask);
		gstate->softmask = NULL;
	}

	fz_concat(&ptm, &pat->matrix, &pat_gstate->ctm);
	fz_invert_matrix(&invptm, &ptm);

	 
	gparent_save = pr->gparent;
	pr->gparent = pr->gtop-1;
	gparent_save_ctm = pr->gstate[pr->gparent].ctm;
	pr->gstate[pr->gparent].ctm = ptm;

	fz_try(ctx)
	{
		 
		local_area = *area;
		fz_transform_rect(&local_area, &invptm);

		fx0 = (local_area.x0 - pat->bbox.x0) / pat->xstep;
		fy0 = (local_area.y0 - pat->bbox.y0) / pat->ystep;
		fx1 = (local_area.x1 - pat->bbox.x0) / pat->xstep;
		fy1 = (local_area.y1 - pat->bbox.y0) / pat->ystep;
		if (fx0 > fx1)
		{
			float t = fx0; fx0 = fx1; fx1 = t;
		}
		if (fy0 > fy1)
		{
			float t = fy0; fy0 = fy1; fy1 = t;
		}

#ifdef TILE
		 
		if (fx1-fx0 > 1 || fy1-fy0 > 1)
#else
		if (0)
#endif
		{
			int cached = fz_begin_tile_id(ctx, pr->dev, &local_area, &pat->bbox, pat->xstep, pat->ystep, &ptm, id);
			if (cached)
			{
				fz_end_tile(ctx, pr->dev);
			}
			else
			{
				gstate->ctm = ptm;
				pdf_gsave(ctx, pr);
				fz_try(ctx)
					pdf_process_contents(ctx, (pdf_processor*)pr, pat->document, pat->resources, pat->contents, NULL);
				fz_always(ctx)
				{
					pdf_grestore(ctx, pr);
					fz_end_tile(ctx, pr->dev);
				}
				fz_catch(ctx)
					fz_rethrow(ctx);
			}
		}
		else
		{
			int x, y;

			 
			x0 = floorf(fx0 + 0.001f);
			y0 = floorf(fy0 + 0.001f);
			x1 = ceilf(fx1 - 0.001f);
			y1 = ceilf(fy1 - 0.001f);
			 
			if (fx1 > fx0 && x1 == x0)
				x1 = x0 + 1;
			if (fy1 > fy0 && y1 == y0)
				y1 = y0 + 1;

			for (y = y0; y < y1; y++)
			{
				for (x = x0; x < x1; x++)
				{
					gstate->ctm = ptm;
					fz_pre_translate(&gstate->ctm, x * pat->xstep, y * pat->ystep);
					pdf_gsave(ctx, pr);
					fz_try(ctx)
						pdf_process_contents(ctx, (pdf_processor*)pr, pat->document, pat->resources, pat->contents, NULL);
					fz_always(ctx)
						pdf_grestore(ctx, pr);
					fz_catch(ctx)
						fz_rethrow(ctx);
				}
			}
		}
	}
	fz_always(ctx)
	{
		pr->gstate[pr->gparent].ctm = gparent_save_ctm;
		pr->gparent = gparent_save;
	}
	fz_catch(ctx)
	{
		fz_rethrow(ctx);
	}

	pdf_grestore(ctx, pr);
}
