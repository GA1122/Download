 pdf_load_xobject(fz_context *ctx, pdf_document *doc, pdf_obj *dict)
  {
         pdf_xobject *form;
  
//        if (!pdf_is_stream(ctx, dict))
//                fz_throw(ctx, FZ_ERROR_SYNTAX, "XObject must be a stream");
// 
         if ((form = pdf_find_item(ctx, pdf_drop_xobject_imp, dict)) != NULL)
                 return form;
 	form->iteration = 0;
 
 	 
 	pdf_store_item(ctx, dict, form, pdf_xobject_size(form));
 
 	form->obj = pdf_keep_obj(ctx, dict);
 
 	return form;
 }