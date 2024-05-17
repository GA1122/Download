void pdf_update_xobject_contents(fz_context *ctx, pdf_document *doc, pdf_xobject *form, fz_buffer *buffer)
{
	pdf_update_stream(ctx, doc, form->obj, buffer, 0);
	form->iteration ++;
}
