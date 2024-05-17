pdf_new_xobject(fz_context *ctx, pdf_document *doc, const fz_rect *bbox, const fz_matrix *mat)
{
	int idict_num;
	pdf_obj *idict = NULL;
	pdf_obj *dict = NULL;
	pdf_xobject *form = NULL;
	pdf_obj *res = NULL;
	pdf_obj *procset;

	fz_var(idict);
	fz_var(dict);
	fz_var(form);
	fz_var(res);

	fz_try(ctx)
	{
		dict = pdf_new_dict(ctx, doc, 0);
		pdf_dict_put_drop(ctx, dict, PDF_NAME_BBox, pdf_new_rect(ctx, doc, bbox));
		pdf_dict_put_drop(ctx, dict, PDF_NAME_FormType, pdf_new_int(ctx, doc, 1));
		pdf_dict_put_drop(ctx, dict, PDF_NAME_Length, pdf_new_int(ctx, doc, 0));
		pdf_dict_put_drop(ctx, dict, PDF_NAME_Matrix, pdf_new_matrix(ctx, doc, mat));

		res = pdf_new_dict(ctx, doc, 0);
		pdf_dict_put(ctx, dict, PDF_NAME_Resources, res);

		procset = pdf_new_array(ctx, doc, 2);
		pdf_dict_put_drop(ctx, res, PDF_NAME_ProcSet, procset);
		pdf_array_push(ctx, procset, PDF_NAME_PDF);
		pdf_array_push(ctx, procset, PDF_NAME_Text);

		pdf_dict_put(ctx, dict, PDF_NAME_Subtype, PDF_NAME_Form);
		pdf_dict_put(ctx, dict, PDF_NAME_Type, PDF_NAME_XObject);

		form = fz_malloc_struct(ctx, pdf_xobject);
		FZ_INIT_STORABLE(form, 1, pdf_drop_xobject_imp);
		form->obj = NULL;
		form->iteration = 0;

		idict_num = pdf_create_object(ctx, doc);
		pdf_update_object(ctx, doc, idict_num, dict);
		idict = pdf_new_indirect(ctx, doc, idict_num, 0);

		pdf_store_item(ctx, idict, form, pdf_xobject_size(form));

		form->obj = pdf_keep_obj(ctx, idict);
	}
	fz_always(ctx)
	{
		pdf_drop_obj(ctx, dict);
		pdf_drop_obj(ctx, res);
		pdf_drop_xobject(ctx, form);
	}
	fz_catch(ctx)
	{
		pdf_drop_obj(ctx, idict);
		fz_rethrow(ctx);
	}

	return idict;
}
