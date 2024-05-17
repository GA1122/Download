make_row_from_rec_and_jsonb(Jsonb *element, PopulateRecordsetState *state)
{
	Datum	   *values;
	bool	   *nulls;
	int			i;
	RecordIOData *my_extra = state->my_extra;
	int			ncolumns = my_extra->ncolumns;
	TupleDesc	tupdesc = state->ret_tdesc;
	HeapTupleHeader rec = state->rec;
	HeapTuple	rettuple;

	values = (Datum *) palloc(ncolumns * sizeof(Datum));
	nulls = (bool *) palloc(ncolumns * sizeof(bool));

	if (state->rec)
	{
		HeapTupleData tuple;

		 
		tuple.t_len = HeapTupleHeaderGetDatumLength(state->rec);
		ItemPointerSetInvalid(&(tuple.t_self));
		tuple.t_tableOid = InvalidOid;
		tuple.t_data = state->rec;

		 
		heap_deform_tuple(&tuple, tupdesc, values, nulls);
	}
	else
	{
		for (i = 0; i < ncolumns; ++i)
		{
			values[i] = (Datum) 0;
			nulls[i] = true;
		}
	}

	for (i = 0; i < ncolumns; ++i)
	{
		ColumnIOData *column_info = &my_extra->columns[i];
		Oid			column_type = tupdesc->attrs[i]->atttypid;
		JsonbValue *v = NULL;
		char	   *key;

		 
		if (tupdesc->attrs[i]->attisdropped)
		{
			nulls[i] = true;
			continue;
		}

		key = NameStr(tupdesc->attrs[i]->attname);

		v = findJsonbValueFromContainerLen(&element->root, JB_FOBJECT,
										   key, strlen(key));

		 
		if (v == NULL && rec)
			continue;

		 
		if (column_info->column_type != column_type)
		{
			getTypeInputInfo(column_type,
							 &column_info->typiofunc,
							 &column_info->typioparam);
			fmgr_info_cxt(column_info->typiofunc, &column_info->proc,
						  state->fn_mcxt);
			column_info->column_type = column_type;
		}
		if (v == NULL || v->type == jbvNull)
		{
			 
			values[i] = InputFunctionCall(&column_info->proc, NULL,
										  column_info->typioparam,
										  tupdesc->attrs[i]->atttypmod);
			nulls[i] = true;
		}
		else
		{
			char	   *s = NULL;

			if (v->type == jbvString)
				s = pnstrdup(v->val.string.val, v->val.string.len);
			else if (v->type == jbvBool)
				s = pnstrdup((v->val.boolean) ? "t" : "f", 1);
			else if (v->type == jbvNumeric)
				s = DatumGetCString(DirectFunctionCall1(numeric_out,
										   PointerGetDatum(v->val.numeric)));
			else if (v->type == jbvBinary)
				s = JsonbToCString(NULL, (JsonbContainer *) v->val.binary.data, v->val.binary.len);
			else
				elog(ERROR, "unrecognized jsonb type: %d", (int) v->type);

			values[i] = InputFunctionCall(&column_info->proc, s,
										  column_info->typioparam,
										  tupdesc->attrs[i]->atttypmod);
			nulls[i] = false;
		}
	}

	rettuple = heap_form_tuple(tupdesc, values, nulls);

	tuplestore_puttuple(state->tuple_store, rettuple);
}
