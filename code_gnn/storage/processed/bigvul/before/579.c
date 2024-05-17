pdf_process_CS(fz_context *ctx, pdf_processor *proc, pdf_csi *csi, int stroke)
{
	if (!proc->op_CS || !proc->op_cs)
		return;

	if (!strcmp(csi->name, "Pattern"))
	{
		if (stroke)
			proc->op_CS(ctx, proc, "Pattern", NULL);
		else
			proc->op_cs(ctx, proc, "Pattern", NULL);
	}
	else
	{
		fz_colorspace *cs;

		if (!strcmp(csi->name, "DeviceGray"))
			cs = fz_keep_colorspace(ctx, fz_device_gray(ctx));
		else if (!strcmp(csi->name, "DeviceRGB"))
			cs = fz_keep_colorspace(ctx, fz_device_rgb(ctx));
		else if (!strcmp(csi->name, "DeviceCMYK"))
			cs = fz_keep_colorspace(ctx, fz_device_cmyk(ctx));
		else
		{
			pdf_obj *csres, *csobj;
			csres = pdf_dict_get(ctx, csi->rdb, PDF_NAME_ColorSpace);
			if (!csres)
				fz_throw(ctx, FZ_ERROR_SYNTAX, "cannot find ColorSpace dictionary");
			csobj = pdf_dict_gets(ctx, csres, csi->name);
			if (!csobj)
				fz_throw(ctx, FZ_ERROR_SYNTAX, "cannot find ColorSpace resource '%s'", csi->name);
			cs = pdf_load_colorspace(ctx, csobj);
		}

		fz_try(ctx)
		{
			if (stroke)
				proc->op_CS(ctx, proc, csi->name, cs);
			else
				proc->op_cs(ctx, proc, csi->name, cs);
		}
		fz_always(ctx)
			fz_drop_colorspace(ctx, cs);
		fz_catch(ctx)
			fz_rethrow(ctx);
	}
}
