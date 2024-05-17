pdf_drop_xobject_imp(fz_context *ctx, fz_storable *xobj_)
{
	pdf_xobject *xobj = (pdf_xobject *)xobj_;
	pdf_drop_obj(ctx, xobj->obj);
	fz_free(ctx, xobj);
}
