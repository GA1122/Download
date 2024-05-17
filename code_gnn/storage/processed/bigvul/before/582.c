pdf_process_SC(fz_context *ctx, pdf_processor *proc, pdf_csi *csi, int stroke)
{
	if (csi->name[0])
	{
		pdf_obj *patres, *patobj, *type;

		patres = pdf_dict_get(ctx, csi->rdb, PDF_NAME_Pattern);
		if (!patres)
			fz_throw(ctx, FZ_ERROR_SYNTAX, "cannot find Pattern dictionary");
		patobj = pdf_dict_gets(ctx, patres, csi->name);
		if (!patobj)
			fz_throw(ctx, FZ_ERROR_SYNTAX, "cannot find Pattern resource '%s'", csi->name);

		type = pdf_dict_get(ctx, patobj, PDF_NAME_PatternType);

		if (pdf_to_int(ctx, type) == 1)
		{
			if (proc->op_SC_pattern && proc->op_sc_pattern)
			{
				pdf_pattern *pat = pdf_load_pattern(ctx, csi->doc, patobj);
				fz_try(ctx)
				{
					if (stroke)
						proc->op_SC_pattern(ctx, proc, csi->name, pat, csi->top, csi->stack);
					else
						proc->op_sc_pattern(ctx, proc, csi->name, pat, csi->top, csi->stack);
				}
				fz_always(ctx)
					pdf_drop_pattern(ctx, pat);
				fz_catch(ctx)
					fz_rethrow(ctx);
			}
		}

		else if (pdf_to_int(ctx, type) == 2)
		{
			if (proc->op_SC_shade && proc->op_sc_shade)
			{
				fz_shade *shade = pdf_load_shading(ctx, csi->doc, patobj);
				fz_try(ctx)
				{
					if (stroke)
						proc->op_SC_shade(ctx, proc, csi->name, shade);
					else
						proc->op_sc_shade(ctx, proc, csi->name, shade);
				}
				fz_always(ctx)
					fz_drop_shade(ctx, shade);
				fz_catch(ctx)
					fz_rethrow(ctx);
			}
		}

		else
		{
			fz_throw(ctx, FZ_ERROR_SYNTAX, "unknown pattern type: %d", pdf_to_int(ctx, type));
		}
	}

	else
	{
		if (proc->op_SC_color && proc->op_sc_color)
		{
			if (stroke)
				proc->op_SC_color(ctx, proc, csi->top, csi->stack);
			else
				proc->op_sc_color(ctx, proc, csi->top, csi->stack);
		}
	}
}
