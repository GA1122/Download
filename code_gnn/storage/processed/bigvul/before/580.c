pdf_process_Do(fz_context *ctx, pdf_processor *proc, pdf_csi *csi)
{
	pdf_obj *xres, *xobj, *subtype;

	xres = pdf_dict_get(ctx, csi->rdb, PDF_NAME_XObject);
	if (!xres)
		fz_throw(ctx, FZ_ERROR_SYNTAX, "cannot find XObject dictionary");
	xobj = pdf_dict_gets(ctx, xres, csi->name);
	if (!xobj)
		fz_throw(ctx, FZ_ERROR_SYNTAX, "cannot find XObject resource '%s'", csi->name);
	subtype = pdf_dict_get(ctx, xobj, PDF_NAME_Subtype);
	if (pdf_name_eq(ctx, subtype, PDF_NAME_Form))
	{
		pdf_obj *st = pdf_dict_get(ctx, xobj, PDF_NAME_Subtype2);
		if (st)
			subtype = st;
	}
	if (!pdf_is_name(ctx, subtype))
		fz_throw(ctx, FZ_ERROR_SYNTAX, "no XObject subtype specified");

	if (pdf_is_hidden_ocg(ctx, csi->doc->ocg, csi->rdb, proc->usage, pdf_dict_get(ctx, xobj, PDF_NAME_OC)))
		return;

	if (pdf_name_eq(ctx, subtype, PDF_NAME_Form))
	{
		if (proc->op_Do_form)
		{
			pdf_xobject *form = pdf_load_xobject(ctx, csi->doc, xobj);

			fz_try(ctx)
				proc->op_Do_form(ctx, proc, csi->name, form, csi->rdb);
			fz_always(ctx)
				pdf_drop_xobject(ctx, form);
			fz_catch(ctx)
				fz_rethrow(ctx);
		}
	}

	else if (pdf_name_eq(ctx, subtype, PDF_NAME_Image))
	{
		if (proc->op_Do_image)
		{
			fz_image *image = pdf_load_image(ctx, csi->doc, xobj);
			fz_try(ctx)
				proc->op_Do_image(ctx, proc, csi->name, image);
			fz_always(ctx)
				fz_drop_image(ctx, image);
			fz_catch(ctx)
				fz_rethrow(ctx);
		}
	}

	else if (!strcmp(pdf_to_name(ctx, subtype), "PS"))
		fz_warn(ctx, "ignoring XObject with subtype PS");
	else
		fz_warn(ctx, "ignoring XObject with unknown subtype: '%s'", pdf_to_name(ctx, subtype));
}
