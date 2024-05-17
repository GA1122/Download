read_hinted_object(fz_context *ctx, pdf_document *doc, int num)
{
	 
	int expected = num;
	int curr_pos;
	int64_t start, offset;

	while (doc->hint_obj_offsets[expected] == 0 && expected > 0)
		expected--;
	if (expected != num)
		DEBUGMESS((ctx, "object %d is unhinted, will search forward from %d", expected, num));
	if (expected == 0)	 
		return 0;

	curr_pos = fz_tell(ctx, doc->file);
	offset = doc->hint_obj_offsets[expected];

	fz_var(expected);

	fz_try(ctx)
	{
		int found;

		 
		do
		{
			start = offset;
			DEBUGMESS((ctx, "Searching for object %d @ %d", expected, offset));
			pdf_obj_read(ctx, doc, &offset, &found, 0);
			DEBUGMESS((ctx, "Found object %d - next will be @ %d", found, offset));
			if (found <= expected)
			{
				 
				doc->hint_obj_offsets[expected] = offset;
				doc->hint_obj_offsets[found] = start;
				doc->hint_obj_offsets[found+1] = offset;
				 
				expected = found+1;
			}
			else
			{
				 
				doc->hint_obj_offsets[expected] = 0;
				doc->hint_obj_offsets[found] = start;
				doc->hint_obj_offsets[found+1] = offset;
				while (doc->hint_obj_offsets[expected] == 0 && expected > 0)
					expected--;
				if (expected == 0)	 
					break;
			}
		}
		while (found != num);
	}
	fz_always(ctx)
	{
		fz_seek(ctx, doc->file, curr_pos, SEEK_SET);
	}
	fz_catch(ctx)
	{
		fz_rethrow_if(ctx, FZ_ERROR_TRYLATER);
		 
		doc->hint_obj_offsets[expected] = 0;
		fz_rethrow(ctx);
	}
	return expected != 0;
}
