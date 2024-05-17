populate_recordset_object_end(void *state)
{
	PopulateRecordsetState *_state = (PopulateRecordsetState *) state;
	HTAB	   *json_hash = _state->json_hash;
	Datum	   *values;
	bool	   *nulls;
	int			i;
	RecordIOData *my_extra = _state->my_extra;
	int			ncolumns = my_extra->ncolumns;
	TupleDesc	tupdesc = _state->ret_tdesc;
	JsonHashEntry *hashentry;
	HeapTupleHeader rec = _state->rec;
	HeapTuple	rettuple;

	 
	if (_state->lex->lex_level > 1)
		return;

	 
	values = (Datum *) palloc(ncolumns * sizeof(Datum));
	nulls = (bool *) palloc(ncolumns * sizeof(bool));

	if (_state->rec)
	{
		HeapTupleData tuple;

		 
		tuple.t_len = HeapTupleHeaderGetDatumLength(_state->rec);
		ItemPointerSetInvalid(&(tuple.t_self));
		tuple.t_tableOid = InvalidOid;
		tuple.t_data = _state->rec;

		 
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
		char	   *value;

		 
		if (tupdesc->attrs[i]->attisdropped)
		{
			nulls[i] = true;
			continue;
		}

		hashentry = hash_search(json_hash,
								NameStr(tupdesc->attrs[i]->attname),
								HASH_FIND, NULL);

		 
		if (hashentry == NULL && rec)
			continue;

		 
		if (column_info->column_type != column_type)
		{
			getTypeInputInfo(column_type,
							 &column_info->typiofunc,
							 &column_info->typioparam);
			fmgr_info_cxt(column_info->typiofunc, &column_info->proc,
						  _state->fn_mcxt);
			column_info->column_type = column_type;
		}
		if (hashentry == NULL || hashentry->isnull)
		{
			 
			values[i] = InputFunctionCall(&column_info->proc, NULL,
										  column_info->typioparam,
										  tupdesc->attrs[i]->atttypmod);
			nulls[i] = true;
		}
		else
		{
			value = hashentry->val;

			values[i] = InputFunctionCall(&column_info->proc, value,
										  column_info->typioparam,
										  tupdesc->attrs[i]->atttypmod);
			nulls[i] = false;
		}
	}

	rettuple = heap_form_tuple(tupdesc, values, nulls);

	tuplestore_puttuple(_state->tuple_store, rettuple);

	 
	hash_destroy(json_hash);
	_state->json_hash = NULL;
}
