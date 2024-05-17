populate_recordset_worker(FunctionCallInfo fcinfo, const char *funcname,
						  bool have_record_arg)
{
	int			json_arg_num = have_record_arg ? 1 : 0;
	Oid			jtype = get_fn_expr_argtype(fcinfo->flinfo, json_arg_num);
	ReturnSetInfo *rsi;
	MemoryContext old_cxt;
	Oid			tupType;
	int32		tupTypmod;
	HeapTupleHeader rec;
	TupleDesc	tupdesc;
	RecordIOData *my_extra;
	int			ncolumns;
	PopulateRecordsetState *state;

	if (have_record_arg)
	{
		Oid			argtype = get_fn_expr_argtype(fcinfo->flinfo, 0);

		if (!type_is_rowtype(argtype))
			ereport(ERROR,
					(errcode(ERRCODE_DATATYPE_MISMATCH),
					 errmsg("first argument of %s must be a row type",
							funcname)));
	}

	rsi = (ReturnSetInfo *) fcinfo->resultinfo;

	if (!rsi || !IsA(rsi, ReturnSetInfo) ||
		(rsi->allowedModes & SFRM_Materialize) == 0 ||
		rsi->expectedDesc == NULL)
		ereport(ERROR,
				(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
				 errmsg("set-valued function called in context that "
						"cannot accept a set")));

	rsi->returnMode = SFRM_Materialize;

	 
	if (get_call_result_type(fcinfo, NULL, &tupdesc) != TYPEFUNC_COMPOSITE)
		ereport(ERROR,
				(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
				 errmsg("function returning record called in context "
						"that cannot accept type record")));

	 
	if (PG_ARGISNULL(json_arg_num))
		PG_RETURN_NULL();

	if (!have_record_arg || PG_ARGISNULL(0))
		rec = NULL;
	else
		rec = PG_GETARG_HEAPTUPLEHEADER(0);

	tupType = tupdesc->tdtypeid;
	tupTypmod = tupdesc->tdtypmod;
	ncolumns = tupdesc->natts;

	 
	my_extra = (RecordIOData *) fcinfo->flinfo->fn_extra;
	if (my_extra == NULL ||
		my_extra->ncolumns != ncolumns)
	{
		fcinfo->flinfo->fn_extra =
			MemoryContextAlloc(fcinfo->flinfo->fn_mcxt,
							   offsetof(RecordIOData, columns) +
							   ncolumns * sizeof(ColumnIOData));
		my_extra = (RecordIOData *) fcinfo->flinfo->fn_extra;
		my_extra->record_type = InvalidOid;
		my_extra->record_typmod = 0;
	}

	if (my_extra->record_type != tupType ||
		my_extra->record_typmod != tupTypmod)
	{
		MemSet(my_extra, 0,
			   offsetof(RecordIOData, columns) +
			   ncolumns * sizeof(ColumnIOData));
		my_extra->record_type = tupType;
		my_extra->record_typmod = tupTypmod;
		my_extra->ncolumns = ncolumns;
	}

	state = palloc0(sizeof(PopulateRecordsetState));

	 
	old_cxt = MemoryContextSwitchTo(rsi->econtext->ecxt_per_query_memory);
	state->ret_tdesc = CreateTupleDescCopy(tupdesc);
	BlessTupleDesc(state->ret_tdesc);
	state->tuple_store = tuplestore_begin_heap(rsi->allowedModes &
											   SFRM_Materialize_Random,
											   false, work_mem);
	MemoryContextSwitchTo(old_cxt);

	state->function_name = funcname;
	state->my_extra = my_extra;
	state->rec = rec;
	state->fn_mcxt = fcinfo->flinfo->fn_mcxt;

	if (jtype == JSONOID)
	{
		text	   *json = PG_GETARG_TEXT_P(json_arg_num);
		JsonLexContext *lex;
		JsonSemAction *sem;

		sem = palloc0(sizeof(JsonSemAction));

		lex = makeJsonLexContext(json, true);

		sem->semstate = (void *) state;
		sem->array_start = populate_recordset_array_start;
		sem->array_element_start = populate_recordset_array_element_start;
		sem->scalar = populate_recordset_scalar;
		sem->object_field_start = populate_recordset_object_field_start;
		sem->object_field_end = populate_recordset_object_field_end;
		sem->object_start = populate_recordset_object_start;
		sem->object_end = populate_recordset_object_end;

		state->lex = lex;

		pg_parse_json(lex, sem);
	}
	else
	{
		Jsonb	   *jb = PG_GETARG_JSONB(json_arg_num);
		JsonbIterator *it;
		JsonbValue	v;
		bool		skipNested = false;
		int			r;

		Assert(jtype == JSONBOID);

		if (JB_ROOT_IS_SCALAR(jb) || !JB_ROOT_IS_ARRAY(jb))
			ereport(ERROR,
					(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
					 errmsg("cannot call %s on a non-array",
							funcname)));

		it = JsonbIteratorInit(&jb->root);

		while ((r = JsonbIteratorNext(&it, &v, skipNested)) != WJB_DONE)
		{
			skipNested = true;

			if (r == WJB_ELEM)
			{
				Jsonb	   *element = JsonbValueToJsonb(&v);

				if (!JB_ROOT_IS_OBJECT(element))
					ereport(ERROR,
							(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
						 errmsg("argument of %s must be an array of objects",
								funcname)));
				make_row_from_rec_and_jsonb(element, state);
			}
		}
	}

	rsi->setResult = state->tuple_store;
	rsi->setDesc = state->ret_tdesc;

	PG_RETURN_NULL();
}
